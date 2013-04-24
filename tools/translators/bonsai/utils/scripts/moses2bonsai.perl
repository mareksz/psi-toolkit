use strict;

while(<STDIN>) {
    chomp;
    my ($source, $target, $scores, $alignment) = split(/\s\|\|\|\s/, $_);
    my @srcSymbols = split(/\s/, $source);
    my @trgSymbols = split(/\s/, $target);
    my @alignment = map {[split(/\-/, $_)]} split(/\s/, $alignment);
    
    my $lhs = pop @srcSymbols;
    $lhs =~ s/^\[(.*)\]$/<$1>/g;
    
    pop @trgSymbols;
    
    my %posmap;
    my $pos = 0;
    my $ntpos = 0;
    foreach my $src (@srcSymbols) {
        if($src =~ /^\[.*\]$/) {
            $src =~ s/^\[([^\]]*)\].*/<$1>/g;
            $src .= "[$ntpos]";
            $posmap{$pos} = $ntpos;
            $ntpos++;
        }
        $pos++;
    }
    
    foreach my $trg (@trgSymbols) {
        if($trg =~ /^\[.*\]$/) {
            $trg = "<X>";
        }
    }
    
    my @talignment;
    foreach $a (@alignment) {
        if($srcSymbols[$a->[0]] =~ /^<.*>\[\d+\]$/) {
            $trgSymbols[$a->[1]] .= "[$posmap{$a->[0]}]"
        }
        else {
            push(@talignment, $a);
        }
    }
    
    $scores = "[". join(",", split(/\s/,$scores)) ."]";
    $alignment = "[". join(",", map { "[$_->[0],$_->[1]]" } @talignment) ."]";
    
    $source = join(" ", @srcSymbols);
    $target = join(" ", @trgSymbols);
    
    my $rule = "$lhs --> $source :: $target";
    print join("\t", $rule, $scores, $alignment), "\n";
}