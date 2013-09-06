#!/bin/bash
#
#  Extract the HiggsMass histogram out of the analysis latinos files
#  S. Luyckx
#  26-08-2013
#
#
#

#
textfile='Input_AllFileNames_ForWeights.txt'
# textfile='Input_AllFileNames_ForWeights_test.txt'
outputpath='/home/sluyckx/cms/HWW2012/2HDM/realCode/MASTERscript/direct_reweighting/output_weights/'

inputpath_ana="~/cms/HWW2012/2HDM/realCode/MASTERscript/fetching_analysis/files_Analysis_HiggsMass_Histograms/"
inputpath_ana_extra="ggH_CPS/"
inputpath_ana="$inputpath_ana$inputpath_ana_extra"

inputpath_lhe="~/cms/HWW2012/2HDM/realCode/MASTERscript/fetching_POWHEG_lhe/files_LHE_POWHEG_HiggsMass_Histograms/"
inputpath_lhe_extra="ggH_CPS/"
inputpath_lhe="$inputpath_lhe$inputpath_lhe_extra"

histoToFit_ana="massHNorm"
histoToFit_lhe="massHNorm"

# reading the textfile lines'
while read p;
do
  # if line starts with # its a comment line so we continue the loop
  if [[ $p == \#* ]]
  then
     continue
  fi
  echo $p
  split_p=( $p )  

  echo "inputfile_ana ${split_p[0]}"
  echo "inputfile_lhe ${split_p[1]}"
  echo "outputfile ${split_p[2]}"

  inputfile_ana=${split_p[0]}
  inputfile_lhe=${split_p[1]}
  outputfile=${split_p[2]}

  tmpfolder='./tmp'
  mkdir $tmpfolder
  cp calculate_Weight.C $tmpfolder
  cd $tmpfolder

  # calculate the weights
  root -l -q calculate_Weight.C+'("'$inputpath_ana'","'$inputfile_ana'","'$inputpath_lhe'","'$inputfile_lhe'","'$histoToFit_ana'","'$histoToFit_lhe'","'$outputfile'")'

  # copy away
  cp $outputfile $outputpath
  echo "Copying weights to : $outputpath"
  cd -

  # remove tmp folder
  rm -rf $tmpfolder
done < $textfile

#
