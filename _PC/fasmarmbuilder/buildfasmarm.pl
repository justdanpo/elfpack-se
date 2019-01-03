#!/bin/perl

use File::Path;
use File::Copy::Recursive qw(rcopy);
use LWP::Simple;
use Mojo::DOM;

rmtree("temp");
rmtree("output");
mkdir("temp");
chdir("temp");

print "reading fasm downloads list\n";
my ($url) = "http://flatassembler.net/download.php";
my $browser = LWP::UserAgent->new;
my $response = $browser->get($url);
if (not $response->is_success) { die("Can't get $url -- ".$response->status_line."\n"); }

my $content = $response->content;
my $dom = Mojo::DOM->new($content);
my @urls = $dom->find('a[href]')->each;
my $gotfasm = false;

foreach $url (@urls)
{
	my $urltext = $url->text;
	my $urlhref = $url->{href};

	if($urlhref =~ /fasmw/)
	{
		if(not $urlhref =~ /http:/)
		{
			$urlhref = "http://flatassembler.net/".$urlhref;
			print "downloading fasmw\n";
			if (RC_OK == getstore($urlhref, "fasmw.zip"))
			{
				$gotfasm = true;
			}
		}
	}
}

if ( not $gotfasm )
{
	die("can't get fasmw");
}

print "downloading fasmarm\n";
getstore("http://arm.flatassembler.net/FASMARM_small.ZIP", "fasmarmsrc.zip") == RC_OK or die("can't get fasmarm");

system("7za x fasmw.zip -ofasm") == 0 or die("can't unpack fasm");
system("7za x fasmarmsrc.zip -ofasmarm") == 0 or die("can't unpack fasmarm");

my @files = glob("../*.patch");
foreach my $file (@files) {
	print "applying patch: $file\n";
	system("patch -p 0 -i $file") == 0 or die("can't patch $file");
	print "\n";
}

rcopy("fasmarm/source", "fasm/source") or die("can't copy a source tree");

chdir("..");

print "compiling\n";
mkdir("output");
system("temp/fasm/FASM temp/fasm/SOURCE/WIN32/FASMARM.ASM output/FASMARM.EXE");



#create FASMARM.ver
my $fasmver = "unknown";
my $fasmarmver = "unknown";
open( FH, "temp/fasmarm/SOURCE/ARMv8.INC" ) or die "can't open ARMv8.INC\n";
while ( <FH> ) {
    if($_ =~ m/ARM_VERSION_STRING equ "(.+)"/g)
    {
	    $fasmarmver = $1;
	    break;
    }
}
close(FH);

open( FH, "temp/fasm/SOURCE/VERSION.INC" ) or die "can't open VERSION.INC\n";
while ( <FH> ) {
    if($_ =~ m/VERSION_STRING equ "(.+)"/g)
    {
	    $fasmver = $1;
	    break;
    }
}
close(FH);

open( FH, ">output/FASMARM.ver" ) or die "can't create ver file\n";
print FH "fasm ", $fasmver, "\nfasmarm ", $fasmarmver, "\n";
close(FH);
