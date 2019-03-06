#!/usr/bin/perl -w

#$SOURCE_DIR ="/home/minerva/cmtuser/Minerva_v10r9p1/Tools/ControlRoomTools/SmartShift";

$sleeptime = 596;
#$sleeptime = 300;
while (1) {
#    $command="at 0625 1425 2225 1321 <<_EOF_";
#    system($command);
    print "Starting to create POT plot at: ";
    $command="date";
    system($command);
    $command = "/home/minerva/cmtuser/Minerva_v10r9p1/Tools/ControlRoomTools/IFBeamData/IFBeamData";
    system($command);
    sleep $sleeptime;
#    $command="_EOF_";
#    system($command);
_EOF_
}

##
# Need to Kerberize: source $HOME/mnvdaqrunscripts/Kerberize
# Run the script by doing perl web_screenshot_veto.pl
#
