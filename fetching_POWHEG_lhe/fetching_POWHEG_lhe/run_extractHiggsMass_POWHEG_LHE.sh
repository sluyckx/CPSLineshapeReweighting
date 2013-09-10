#!/bin/bash
#
#  Extract the HiggsMass histogram out of the lhe tarred files
#  S. Luyckx
#  26-08-2013
#
#
#

textfile='Input_AllFileNames.txt'

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
  #mass           ${split_p[0]}
  #inputpath/file ${split_p[1]}
  #outputfile     ${split_p[2]}
  #outputpath     ${split_p[3]}

  mass=${split_p[0]}
  inputfile=${split_p[1]}
  outputfile=${split_p[2]}
  outputpath=${split_p[3]}
  #tmpfile
  lhe2rootfile='lhe2root_intermediateFile.root'

  tmpfolder='./tmp'
  mkdir $tmpfolder
  cp lhe2root.py $tmpfolder
  cp extractHiggsMass_POWHEG_LHE.C $tmpfolder
  cd $tmpfolder
  # will untar to this directory
  tar -zxf $inputfile pwgevents.lhe

  # make a lhe2rootfile with E, M, Px, pY
  python lhe2root.py pwgevents.lhe $lhe2rootfile
  # make the HiggsMass Histogram for the HiggsParticles and save the histogram
  root -l -q extractHiggsMass_POWHEG_LHE.C+'("'$lhe2rootfile'",'$mass',"'$outputfile'")'

  # copy away
  cp $outputfile $outputpath 
  echo "Copied all the files to $outputpath"
  cd - 
 
  # remove tmp folder

  rm -rf $tmpfolder

done < $textfile
#
