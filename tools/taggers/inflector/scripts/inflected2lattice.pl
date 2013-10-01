use strict;
use Encode qw(encode_utf8);
use Getopt::Long;

binmode(STDIN, ":utf8");
binmode(STDOUT, ":utf8");

my $EDGES = 1;
my $START = 0;

my $FRAG_START = 0;

my $LANG = "xx";
GetOptions(
    "l|lang=s" => \$LANG
);

my @FRAG;

print "## beg. len.  text    tag    annot.text    annotations\n";

while(<STDIN>) {
    chomp;
    if($_ eq "<s>") {
        $FRAG_START = $START;
        next;   
    }
    elsif($_ eq "</s>") {
        my $text = join("_", @FRAG);
        
        next if not ($START - $FRAG_START);
        
        print sprintf("%02d %04d %02d", $EDGES, $FRAG_START, $START - $FRAG_START);
        print "    ";
        print "$text";
        print "    ";
        print "!$LANG,segment    $text     sen";
        print "\n";
        
        @FRAG = ();
        
        $EDGES++;
        next;   
    }
    elsif($FRAG_START != $START) {
        my $LENGTH = 1;
        print sprintf("%02d %04d %02d", $EDGES, $START, $LENGTH);
        print "    ";
        print "_";
        print "    ";
        print "!$LANG,token    _    B";
        print "\n";
        
        $START += $LENGTH;
        $EDGES++;
    }
    
    my ($token, $lemma, @tags) = split;
    push(@FRAG, $token);
    
    my $LENGTH = length(encode_utf8($token));
    
    print sprintf("%02d %04d %02d", $EDGES, $START, $LENGTH);
    print "    ";
    print "$token";
    print "    ";
    print "!$LANG,token    $token    T";
    print "\n";

    my $PREV = $EDGES;
    $EDGES++;

    #my $norm = $lemma =~ /^\p{Ll}/ ? lc($token) : $token;
    #
    #print sprintf("%02d %04d %02d", $EDGES, $START, $LENGTH);
    #print "    ";
    #print "$token";
    #print "    ";
    #print "!$LANG,normalization,train    $norm    term[$PREV]";
    #print "\n";
    #
    #$PREV = $EDGES;
    #$EDGES++;
    #
    #print sprintf("%02d %04d %02d", $EDGES, $START, $LENGTH);
    #print "    ";
    #print "$token";
    #print "    ";
    #print "!$LANG,lemma,train    $lemma    word[$PREV]";
    #print "\n";
    #
    #$PREV = $EDGES;
    #$EDGES++;
    #
    #print sprintf("%02d %04d %02d", $EDGES, $START, $LENGTH);
    #print "    ";
    #print "$token";
    #print "    ";
    #print "!$LANG,lexeme,train    $lemma+$tag    ${tag}[$PREV]";
    #print "\n";
    #    
    #$PREV = $EDGES;
    #$EDGES++;
       
    foreach my $tag (@tags) {
        print sprintf("%02d %04d %02d", $EDGES, $START, $LENGTH);
        print "    ";
        print "$token";
        print "    ";
        print "!$LANG,form,inflector,train    $lemma    ${tag}[$PREV]";
        print "\n";
        
        $EDGES++;
    }
    $START += $LENGTH;

}
    