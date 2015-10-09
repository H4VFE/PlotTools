from ROOT import *
import sys

""" Accepts a root file containing H4 test beam data and several options on
    what to draw in that file. 'event' specifies what event number to draw.
    'c1' and 'c2' specify which two channels to compare. And group specifies 
    which group to draw (0 or 1), although 1 is usually the better choice.
    Draws two graphs showing the data from each channel for the given event.

    To Run:
    - default
    python DrawSamples.py 
    - your own root file
    python DrawSamples.py myawesomedata.root
    - specifying channels and event number
    python DrawSamples.py myawesomedata.root 52 3 6

"""
def DrawSamples(infile = "run3881_12.root", event = 20, c1 = 1, c2 = 2, group = 1):
    # Access the tree
    inFile  = TFile(infile, "Read")
    h4tree = inFile.Get("H4tree")

    canvas = TCanvas()
    canvas.Divide(2,1)

    # Draw the same given event from two different channels in one canvas
    canvas.cd(1)
    h4tree.Draw("digiSampleValue:0.2*(digiSampleIndex)",
                "digiChannel == " + str(c1) +" && digiGroup == " + str(group) +
                "&& digiSampleIndex<1000 && digiSampleValue < 4000 && \
                evtNumber == " + str(event))
    canvas.cd(2)
    h4tree.Draw("digiSampleValue:0.2*(digiSampleIndex)",
                "digiChannel == " + str(c2) +" && digiGroup == " + str(group) +
                "&& digiSampleIndex<1000 && digiSampleValue < 4000 && \
                evtNumber == " + str(event))

    canvas.SaveAs("./test_beam_2015.png")

nargs = len(sys.argv)
if(nargs == 1):
    DrawSamples()
elif(nargs == 2):
    DrawSamples(sys.argv[1])
elif(nargs == 3):
    DrawSamples(sys.argv[1], sys.argv[2])
elif(nargs == 4):
    DrawSamples(sys.argv[1], sys.argv[2], sys.argv[3])
elif(nargs == 5):
    DrawSamples(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
elif(nargs == 6):
    DrawSamples(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
