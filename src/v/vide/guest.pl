#!/usr/bin/perl
#  ^
# / \
#  |   Modify the line the arrow is pointing to, to the correct
#  |   location of Perl v5 or better.
#Uninet######################################################################
#   w  w  w  .  u  n  i  n  e  t  s  o  l  u  t  i  o  n  s  .  c  o   m    #
# UNINET PERL COMPANY(TM) PRESENTS                                          #
# GuestCall SR1 (1.1)                                                       #
# Compact, Powerful, GuestCall.                                             #
# ------------------------------------------------------------------------- #
# (C)1997-98 Uninet Communications Co., All Rights Reserved.                #
#                                                                           #
# This program is free software; you can redistribute it and/or             #
# modify it under the terms of the GNU General Public License               #
# as published by the Free Software Foundation; either version 2            #
# of the License, or (at your option) any later version.                    #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              # 
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with this program; if not, write to the                             #
#                Free Software Foundation, Inc.                             #
#                59 Temple Place - Suite 330                                #
#                Boston, MA  02111-1307, USA.                               #
#                                                                           #
# -->A full copy of the GNU General Public License is included in gpl.txt   #
# ------------------------------------------------------------------------- #
# Need Support? We offer FREE technical support!                            #
# Email us at support@uninetsolutions.com for quick, free support today!    # 
# ------------------------------------------------------------------------- #
# Modification History:                                                     #
# 1.0: 5/5/1997  GuestCall Released.                                        #
# SR1: 5/6/1997  HTML Creation Error Fixed.                                 #
#######################################################################Uninet 
#Heres how to set me up: just follow the instructions.
#1. What is your site's name?
$name = "My neat site";

#2.What color background do you want?
$color = "white";

#3. Where is the data file?
$dtfl = "gbook.txt";

#4. Where is the log file located?
$lognn = "log.txt";
##################################
#your done! your done! your done!#
##################################
@months = ('Jan','Feb','Mar','Apr','May','June',
		 'July','Aug','Sept','Oct','Nov','Dec');
@days = ('Sunday','Monday','Tuesday','Wednesday','Thursday','Friday','Saturday');
($sec,$min,$hour,$mday,$mon,$year,$wday) = (localtime(time))[0,1,2,3,4,5,6];
if ($year > "96") {
 $yd = "19$year";
}
else {
 $yd = "20$year";
}
$date = " $days[$wday], $months[$mon] $mday, $yd at $hour:$min";
open (data,"$dtfl");
@dtf = <data>;
close (data);
if (@dtf eq ""){
$i = 0;
}
else{
$i = 0;
foreach $dtf (@dtf){
$i = $i + 1;
}
}
read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
@pairs = split(/&/, $buffer);
foreach $pair (@pairs)
{
    ($name, $value) = split(/=/, $pair);
    $value =~ tr/+/ /;
    $value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
    $value =~ s/~!/ ~!/g; 
    $FORM{$name} = $value;
}
$compare = "$FORM{'snd'}";
if ($compare eq ""){
if ($ENV{'QUERY_STRING'} eq "log") {
open (laggg, "$lognn");
@lg = <laggg>;
close(loggg); 
print ("Content-type: text/html\n\n");
print "<html><head><title>Log file</title></head><body bgcolor = '$color'>You currently have $i messages.<BR><table width = 100%><td>Message No.</td><td>User's Name</td><td>User's Client</td><td>Date written</td><TR>";
print "<td colspan = 5><HR></td><tr>";
print "@lg</table><BR>(C)1997 TWN Perl Co. Processed on $date.</body></html>";
}
else{
#Prepare page
print ("Content-type: text/html\n\n");
print "<head><title>$name Guest Book</title></head><body bgcolor = '$color'><center><h2>$name</h2><h4>Guest Book</h4></center><HR>";
print "Currently there are $i messages.<BR>@dtf<BR><HR><FORM ACTION='$SCRIPT_NAME' METHOD=POST>Your Name:<Input type = 'text' name = 'name' SIZE='45'><BR>Email Address:<Input type = 'text' name = 'email' SIZE='45'><BR>How did you find us:<Input type = 'text' name = 'how' SIZE='45'><BR>Comments:<BR> <TEXTAREA width = 100% height = 100% name='comment' wrap=soft cols=60 rows=15></TEXTAREA> <Input type = 'Hidden' name = 'snd' value='2444211'><BR><center><input type='submit' name='submit' value='Submit'> <input type='reset'  name='reset'  value='Clear Form'></center><BR></form><BR>Script by <a href = 'http://www.serve.com/twn/cgi/'>TWN Perl Company</a>.</body></html>";
}
}
else{
#Add message.
$i = $i + 1;
open (mordat, "$dtfl");
@datt = <mordat>;
close(mordat);

open (mordat, ">$dtfl");
print mordat "<B>Name:</b><a href = \"mailto:$FORM{'email'}\">$FORM{'name'}</a><BR><b>How did you find us:</b>$FORM{'how'}<BR>$FORM{'comment'}<BR>$date<BR>\n@datt";
close(mordat);

open (logdtt, "$lognn");
@mrrdt = <logdtt>;
close(logdat);

open (logdat, ">$lognn");
print logdat "<td>$i</td><td>$FORM{'name'}</td><td>$ENV{'HTTP_USER_AGENT'}</td><td>$date</td><TR>\n@mrrdt";
close(logdat);

print ("Content-type: text/html\n\n");
print "<html><head><title>Message Added</title></head><body bgcolor = '$color'><H1>Message Added</h1><BR>Here is what you added:<BR><B>Name:</B>$FORM{'name'}<BR><B>Email Address:</B>$FORM{'email'}<BR><B>How did you find us:</B>$FORM{'how'}<BR><B>Comments:</B><BR>$FORM{'comment'}<BR><a href = \"$ENV{'HTTP_REFERER'}\">Back to the guest book</a><HR>Script by <a href = 'http://www.serve.com/twn/cgi/'>TWN Perl Company</a></body></html>";
}
#Thats all!
