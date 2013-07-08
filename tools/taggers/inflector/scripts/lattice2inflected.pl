use strict;

my $sent = 0;
my $first = 0;
my $begin = 1;

my $token;
my $normal;
my $lemma;
my $tokenType;
my @forms;

while (<STDIN>) {
    chomp;
    
    if (/^## beg/) {
        $sent = 1;
    }
    
    my ($lp, $beg, $len, $text, $tags, $antext, $annot) = split(/\s+/, $_);
    if ($annot eq "T" or $annot eq "I" or $annot eq "X") {
    
        if ($first) {
            if (not $begin) {
                print "</s>\n";
            }
            
            print "<s>\n";
            $first = 0;
            $begin = 0;
        }
        
    
        if ($tokenType and $tokenType eq "T") {
            if (not @forms) {
                $lemma = $token;
            }
            
            print "$lemma\t$token\t";
            print join("\t", sort map { my @t = split(/,/, $_->[1]); join(",", $t[0], sort @t[1 .. $#t]) } @forms), "\n";
        }

        if ($tokenType and $tokenType ne "T") {
            print "$token\t$token\n";
        }
    
        @forms = ();
        
        $token = $antext;
        $tokenType = $annot;
        
        if ($sent) {
            $first = 1;
            $sent = 0;
        }
        
        
    }
    
    if ($tags =~ /\blemma\b/) {
        $lemma = $antext;
    }
    
    if ($tags =~ /\bform\b/) {
        $annot =~ s/\[\d+\]//;
        push(@forms, [$antext, $annot]);
    }
    
    
}
