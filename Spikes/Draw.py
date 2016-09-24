from ROOT import *
import sys

""" Accepts a root file containing H4 test beam data and several options on
    what to draw in that file. 'event' specifies what event number to draw.
    'channel1' and 'channel2' specify which two channels to compare. And group specifies 
    which group to draw (0 or 1), although 1 is usually the better choice.
    Draws two graphs showing the data from each channel for the given event.

    To Run:
    - default
    python Draw.py 
    - your own root file
    python Draw.py myawesomedata.root
    - specifying channels and event number
    python Draw.py myawesomedata.root 52 3 6

"""
def Draw(infile = "2.root", event = -1, channel1 = 0, channel2 = 1, group = 3):
    # Access the tree
    inFile  = TFile(infile, "Read")
    h4tree = inFile.Get("H4tree")

    canvas = TCanvas()
    canvas.Divide(2,1)

    if event != -1 :
      # Draw the same given event from two different channels in one canvas
      canvas.cd(1)
      h4tree.Draw("digiSampleValue:0.2*(digiSampleIndex)",
                  "digiChannel == " + str(channel1) +" && digiGroup == " + str(group) +
                  "&& digiSampleIndex<1000 && digiSampleValue < 4000 && \
                  evtNumber == " + str(event))
      canvas.cd(2)
      h4tree.Draw("digiSampleValue:0.2*(digiSampleIndex)",
                  "digiChannel == " + str(channel2) +" && digiGroup == " + str(group) +
                  "&& digiSampleIndex<1000 && digiSampleValue < 4000 && \
                  evtNumber == " + str(event))

    else :
      # Draw the same all events from two different channels in one canvas
      canvas.cd(1)
      h4tree.Draw("digiSampleValue:0.2*(digiSampleIndex)",
                  "digiChannel == " + str(channel1) +" && digiGroup == " + str(group) +
                  "&& digiSampleIndex<1000 && digiSampleValue < 4000 ")
      canvas.cd(2)
      h4tree.Draw("digiSampleValue:0.2*(digiSampleIndex)",
                  "digiChannel == " + str(channel2) +" && digiGroup == " + str(group) +
                  "&& digiSampleIndex<1000 && digiSampleValue < 4000 ")
      
      
    canvas.SaveAs("./test_beam_2016_spike_" + infile + ".root")

nargs = len(sys.argv)
if(nargs == 1):
    Draw()
elif(nargs == 2):
    Draw(sys.argv[1])
elif(nargs == 3):
    Draw(sys.argv[1], sys.argv[2])
elif(nargs == 4):
    Draw(sys.argv[1], sys.argv[2], sys.argv[3])
elif(nargs == 5):
    Draw(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
elif(nargs == 6):
    Draw(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
