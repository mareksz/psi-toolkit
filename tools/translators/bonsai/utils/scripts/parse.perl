#!/usr/bin/perl
use strict;

use File::Basename;
use File::Spec::Functions;
use File::Spec::Unix;

use Parallel::ForkManager;
use File::Temp qw(tempdir);

my $PSIBUILD = "/home/marcinj/Poleng/Psi/psi-toolkit/build";
my $PSIFRAMEWORK = "$PSIBUILD/framework";
my $PARSE = "$PSIFRAMEWORK/psi-pipe --line-by-line puddle --lang pl ! bracketing-writer --skip-symbol-edges --opening-bracket '<tree_label=\"%c\">' --closing-bracket \"</tree>\" --filter parse lexeme frag";

my $input  = File::Spec->rel2abs($ARGV[0]);
my $output = File::Spec->rel2abs($ARGV[1]);

my $dir = File::Temp->newdir(CLEANUP => 1);
my $tempdir = $dir->dirname;

`split -a 5 -d -l 1000 $input $tempdir/input.`;

my $fm = new Parallel::ForkManager(6);

foreach my $file (<$tempdir/input.*>) {
    $fm->start() and next;

    chdir $PSIBUILD;
    `$PARSE < $file > $file.parsed`;

    $fm->finish()
}

$fm->wait_all_children();

`cat $tempdir/input.*.parsed > $output`;