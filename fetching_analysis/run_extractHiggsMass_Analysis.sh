#!/bin/bash
#
#  Extract the HiggsMass histogram out of the analysis latinos files
#  S. Luyckx
#  26-08-2013
#
#
#


#
textfile='Input_AllFileNames_Analysis.txt'
outputpath='/home/sluyckx/cms/HWW2012/2HDM/realCode/MASTERscript/fetching_analysis/files_Analysis_HiggsMass_Histograms/ggH_CPS'

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
  echo "mass           ${split_p[0]}"
  echo "inputpath/file ${split_p[1]}"
  echo "outputfile     ${split_p[2]}"

  mass=${split_p[0]}
  inputfile=${split_p[1]}
  outputfile=${split_p[2]}

  tmpfolder='./tmp'
  mkdir $tmpfolder
  cp extractHiggsMass_Analysis.C $tmpfolder
  cd $tmpfolder

  # make the HiggsMass Histogram for the HiggsParticles and save the histogram
  root -l -q extractHiggsMass_Analysis.C+'('$mass',"'$inputfile'","'$outputfile'")'

  # copy away
  cp $outputfile $outputpath
  echo "Copying histogram to : $outputpath"
  cd -

  # remove tmp folder
  rm -rf $tmpfolder

done < $textfile

#
