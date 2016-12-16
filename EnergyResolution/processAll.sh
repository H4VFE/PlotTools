#Creates scripts for and processes all runs in the list
#Must be on lxplus, with proper config file settings

for i in 5269 5271 5272 5283
do
python submitBatch.py -r $i -s ./ --dryrun
bash 16-11-2016_ntuples_v1/H4Reco_1nh_$i\.sh
done
