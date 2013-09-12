#!/bin/bash

#

#inputpath="~/cms/HWW2012/2HDM/realCode/rootfiles/v0/analysis/"
inputfile="massH_500GeV_analysis.root"
#inputfile="massH_500GeV_lhe.root"


histoToFit="massH"

fit_kind=1

root -l -q fit_some_BW.C+'("'$inputpath'","'$inputfile'","'$histoToFit'",'$fit_kind')' 


#inputpath="~/cms/HWW2012/2HDM/realCode/rootfiles/v0/POWHEG_lhe/"
inputfile="massH_500GeV_lhe.root"
root -l -q fit_some_BW.C+'("'$inputpath'","'$inputfile'","'$histoToFit'",'$fit_kind')' 
#
