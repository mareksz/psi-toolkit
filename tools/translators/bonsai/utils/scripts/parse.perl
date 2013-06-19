#!/usr/bin/perl
use strict;

use File::Basename;
use File::Spec::Functions;
use File::Spec::Unix;

use Parallel::ForkManager;
use Sys::CPU;
use File::Temp qw(tempdir);

use Getopt::Long;

my $PSIBUILD = ".";
my $LANG = "pl";
my $INPUT;
my $OUTPUT;
my $FORK = 0;

GetOptions(
    "psi=s" => \$PSIBUILD,
    "lang=s" => \$LANG,
    "input=s" => \$INPUT,
    "output=s" => \$OUTPUT,
    "fork=s" => \$FORK,
);

my $PSIBUILD_ = File::Spec->rel2abs($PSIBUILD);

die "No input file given.\n" if(not $INPUT);
die "No output file given.\n" if(not $OUTPUT);
die "Path '$PSIBUILD' does not exist" if(not -e "$PSIBUILD");

my $PSIFRAMEWORK_ = "$PSIBUILD_/framework";
my $PARSE_ = "$PSIFRAMEWORK_/psi-pipe --line-by-line puddle --lang $LANG ! bracketing-writer --skip-symbol-edges --opening-bracket '<tree_label=\"%c\">' --closing-bracket \"</tree>\" --tags parse lexeme --disambig";

my $INPUT_  = File::Spec->rel2abs($INPUT);
my $OUTPUT_ = File::Spec->rel2abs($OUTPUT);

if($FORK eq "all") {
    $FORK = Sys::CPU::cpu_count();
}
else {
    $FORK = $FORK + 0;
}

if($FORK > 1) {
    print STDERR "Splitting data into $FORK parts\n";
    my $dir = File::Temp->newdir(CLEANUP => 1);
    my $tempdir = $dir->dirname;
    
    `split -a 5 -d -l 1000 $INPUT_ $tempdir/input.`;
    
    my $fm = new Parallel::ForkManager($FORK);
    
    print STDERR "Running $FORK parsing processes\n";
    my $num = 0;
    foreach my $file (<$tempdir/input.*>) {
        $num++;
        $fm->start() and next;
    
        print STDERR "Starting parsing process $num\n";
        chdir $PSIBUILD_;
        `$PARSE_ < $file > $file.parsed`;
        print STDERR "Finished parsing process $num\n";
    
        $fm->finish()
    }
    
    $fm->wait_all_children();

    print STDERR "Finished parsing\n";
    print STDERR "Concatenating output\n";
    
    `cat $tempdir/input.*.parsed > $OUTPUT_`;
    print STDERR "Done\n";
}
elsif($FORK == 1 or $FORK == 0) {
    print STDERR "Starting parsing process\n";
    chdir $PSIBUILD_;
    `$PARSE_ < $INPUT_ > $OUTPUT_`;
    print STDERR "Done\n";
}
else {
    die "$FORK is not a valid number of processes.\n";
}