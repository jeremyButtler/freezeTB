#!/usr/bin/sh

scriptDirStr="$(dirname "$0")";
cd "$scriptDirStr" || exit;
retDirStr="$(basename "$scriptDirStr" | sed 's/\.\///;')";

cd ../freezeTBSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh"  freezeTB mainFreezeTB freezeTB
cd "../$retDirStr" || exit;

cd ../mkMiruTblSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh"  mkMiruTbl mkMiruTbl memwater fileFun charCp ../genLib/endLine.h ../ftbVersion.h
cd "../$retDirStr" || exit;

cd ../swapDbRefSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh"  swapDbRef swapDbRef refSwap charCp ../ftbVersion.h
cd "../$retDirStr" || exit;

cd ../tbAmrSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh"  tbAmr tbAmr checkAmr ../genFreezeTB/tbAmrDefs.h ../ftbVersion.h
cd "../$retDirStr" || exit;

cd ../tbMiruSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh"  tbMiru tbMiru miruFx ../ftbVersion.h
cd "../$retDirStr" || exit;

cd ../tbSpolSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh"  tbSpol tbSpol spolFind ../ftbVersion.h
cd "../$retDirStr" || exit;

cd ../whoToAmrSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh"  whoToAmr mainWhoToAmr whoToAmr ../ftbVersion.h
cd "../$retDirStr" || exit;

