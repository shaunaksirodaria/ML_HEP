Author:- Thorben Quast
https://github.com/ThorbenQuast

import numpy as np
import ROOT
import array

signalExpression="beamEnergy"
branches = ["EAll_layer"+str(i) for i in range(1, 41)]

class Dataset(object):
  def __init__(self, path):
    self._data = []     #inputs
    self._targets = []  #desired output
    
    self._data_eval = []     #inputs
    self._targets_eval = []  #desired output

    for energy in [20, 50, 80, 100, 120, 200, 250, 300]:
      fpath = path.replace("<ENERGY>", str(energy))
      print "Reading",fpath
      infile = ROOT.TFile(fpath, "READ")
      tree = infile.Get("observablentupler/variables")

      #### Conversion of the root tuple to numpy arrays ###
      ### Alternative is root_numpy: https://pypi.python.org/pypi/root_numpy
      branch_fillers = []
      for b in range(len(branches)):
        branch = branches[b]
        branch_filler = array.array( 'd', [0.])
        tree.SetBranchAddress(branch, branch_filler)
        branch_fillers.append(branch_filler)
      target_filler = array.array( 'd', [0.])
      tree.SetBranchAddress(signalExpression, target_filler)

      for i in range(tree.GetEntries()):
        tree.GetEntry(i)
        entry = []
        for b in range(len(branches)):
          entry.append(branch_fillers[b][0])
        self._data.append(np.array(entry))
        self._targets.append(np.array([target_filler[0]]))
    
    self._data = np.array(self._data)
    self._targets = np.array(self._targets)
    
    #### Some book-keeping
    self._num_examples = len(self._data)
    self._dim_input = len(self._data[0])
    self._index_in_epoch = 0
    self._epochs_completed = 0

    #shuffling
    print "Shuffling"
    perm = np.arange(self._num_examples)
    np.random.shuffle(perm)
    self._data = self._data[perm]
    self._targets = self._targets[perm]

    #splitting into train and evaluation set
    nTrain = int(0.9 * self._num_examples)
    print nTrain,"/",self._num_examples,"showers for training"
    self._data_eval = self._data[nTrain+1:self._num_examples]
    self._targets_eval = self._targets[nTrain+1:self._num_examples]
    self._data = self._data[0:nTrain]
    self._targets = self._targets[0:nTrain]

    self._num_examples = nTrain


  def get_N_node(self):
    return self._dim_input

  #this function returns a subset (N=batch_size) of the available data
  #after one iteration through the whole set, it is shuffled
  def next_batch(self, batch_size):
    start = self._index_in_epoch
    self._index_in_epoch += batch_size
    if self._index_in_epoch >= self._num_examples:
      # Finished epoch
      self._epochs_completed += 1
      # Shuffle the data
      perm = np.arange(self._num_examples)
      np.random.shuffle(perm)
      self._data = self._data[perm]
      self._targets = self._targets[perm]
      # Start next epoch
      start = 0
      self._index_in_epoch = batch_size
      assert batch_size <= self._num_examples
    end = self._index_in_epoch
    return self._data[start:end], self._targets[start:end]


def read_data_set(path):
  return Dataset(path)
