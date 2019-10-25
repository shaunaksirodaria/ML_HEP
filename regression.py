Author:- Thorben Quast
https://github.com/ThorbenQuast

import time

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--displayCost", help="Display the cost function evaluated on the test set (requires matplotlib)", type=bool, default=True)
args = parser.parse_args()

########## 1) Reading the input tuples and clear the plot folder ##########
import readData

dataSet = readData.read_data_set("datawith<ENERGY>.root")
output_folder = "./outputs"
import shutil, os
if os.path.exists(output_folder):
	shutil.rmtree(output_folder)
os.mkdir(output_folder)

########## 
N_iterations = 50000
print_every = 1000
doPlotting = args.displayCost
plot_every = 20

minimizing_step = 0.001	#optimizer
batch_size = 10000

N_inputs = dataSet.get_N_node()

#internal parameters are initialized with gaussian distributed random numbers
init_mu=0.001
init_stddev=0.1

########## 2) Setting up the network ##########
#these few lines define the architecture of the neural network using tensorflow
import tensorflow as tf


with tf.device('/cpu:0'):		#if one has the gpu version of tensorflow installed, one could indicate tf.device('/gpu:0') instead
	x = tf.placeholder(tf.float32, [None, dataSet.get_N_node()])
	W01 = tf.Variable(tf.random_normal([N_inputs, 1], mean=0.05, stddev=init_stddev))
	y = tf.matmul(x, W01)
	
	y_ = tf.placeholder(tf.float32, [batch_size, 1])


########## 3) Initializing the training ##########
##########
#cost function to be minimized is a chi2 measure
minimized_function = tf.reduce_sum(tf.pow((y-y_),2)/y_)
##########
train_step = tf.train.AdamOptimizer(minimizing_step).minimize(minimized_function)
init = tf.initialize_all_variables()

sess = tf.Session()
sess.run(init)


#setup the saver: https://www.tensorflow.org/programmers_guide/variables
saver = tf.train.Saver()

########## 4) Optional: Live plotting of the cost function ########## 
if doPlotting:
	import matplotlib.pyplot as plt
	import numpy as np
	plt.ion()
	fig, ax = plt.subplots(nrows=1, ncols=1)
	ax.set_xlabel("# epoch of training")
	ax.set_ylabel("$ \chi^2 $")
	x_epoch, y_chi2 = [],[]
	plot, = ax.plot(x_epoch, y_chi2)
	y_max=0

from ROOT import TCanvas, TH2F, TText, TF1
import ROOT
ROOT.gROOT.SetBatch(True)


########## 5) Perform the training ########## 
for i in range(N_iterations+1):
	#load a subset of the training data
	batch_xs, batch_ys = dataSet.next_batch(batch_size)

	#run the training step, i.e. the minimization of the cost function by varying the internal parameters (W01, b01, W12, b12, W23, b23, W34, b34)
	sess.run(train_step, feed_dict={x: batch_xs, 
		y_: batch_ys})
	 
	#compute the cost after this miniization step 
	chi2_test = sess.run(minimized_function, feed_dict={
			x: batch_xs, y_: batch_ys})/(batch_size)

	########## 6a) Live plotting of the cost function evaluated on the training sample ########## 
	if not i%plot_every and doPlotting:
		x_epoch.append(i)
		y_chi2.append(chi2_test)
		y_max = max(chi2_test, y_max)
		ax.axis([0, N_iterations, 0, y_max])
		plot.set_xdata(np.array(x_epoch))
		plot.set_ydata(np.array(y_chi2))
		plt.yscale("log")
		plt.draw()
		plt.pause(0.001)


	########## 6b) Evaluate the performance and visualise in a 2D histogram ########## 
	outfile = ROOT.TFile("%s/outfile.root"%output_folder, "RECREATE")
	outfile.Close()
	if not i%print_every:
		#store the variables and operations of the network
		saver.save(sess, "%s/model_%s.ckpt" % (output_folder, str(i)))

		y_eval = sess.run(y, feed_dict={x: dataSet._data_eval})
		
		hist = TH2F("N_{epochs}="+str(i), "N_{epochs}="+str(i), 29, 14.5, 304.5, 1000, min(y_eval)[0], max(y_eval)[0])
		
		for k in range(len(y_eval)):
			hist.Fill(dataSet._targets_eval[k][0], y_eval[k])
		canvas = TCanvas("N_{epochs}="+str(i), "N_{epochs}="+str(i), 800, 400)
		hist.GetXaxis().SetTitle("real target")
		hist.GetYaxis().SetTitle("evaluated target")
		ROOT.gStyle.SetOptStat(0)
		ROOT.gStyle.SetOptFit(1)
		pol1 = TF1("pol1","pol1", 0.4, 2.0)
		profile = hist.ProfileX()
		profile.Fit(pol1)
		profile.SetLineWidth(3)

		m = round(pol1.GetParameter(1),3)
		b = round(pol1.GetParameter(0), 3)
		hist.SetTitle("m="+str(m)+", b="+str(b)+", N_{epochs}="+str(i))
		hist.Draw("COLZ")
		profile.Draw("SAME")
		canvas.Update()
		canvas.Print("%s/%s.pdf" % (output_folder, str(i)))

		outfile = ROOT.TFile("%s/outfile.root"%output_folder, "UPDATE")
		hist.Write()
		outfile.Close()

		print "[%s] %s out of %s completed: chi2=%s" % (time.ctime(), i, N_iterations, chi2_test)
		print "Parameters:"
		print sess.run(W01)
