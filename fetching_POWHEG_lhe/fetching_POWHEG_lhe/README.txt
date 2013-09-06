# 1) Take LHE files from EOS or Brussels:
#   EOS :    /eos/cms/store/caf/user/rgerosa/powheg_lhe_production/
#   Copied to Brussels here : ~/files_LHE_POWHEG_HiggsMass
#
# 2) Untar and run lhe2root.py
#
# 3) Run root -l -q extractHiggsMass_POWHEG_LHE.C+ on the desired mass + lhe2root file to receive the HiggsMass histogram
