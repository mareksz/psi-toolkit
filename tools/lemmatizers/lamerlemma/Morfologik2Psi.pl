use strict;
use Data::Dumper;

my %map;
while(<DATA>) {
    chomp;
    my ($tag, $name) = split;
    $map{$tag} = $name;
}

while(<STDIN>) {
    chomp;
    print STDERR $.,"\n" if($. % 100000 == 0);
    my ($form, $lemma, $tag) = split(/\t/, $_);
    $form =~ s/\ /_/g;
    $lemma =~ s/\ /_/g;
    my @parses = parse($tag);
    foreach my $parse (@parses) {
        my ($pos, @features) = parse_to_features($parse, $_);
        print join("\t", $form, $lemma, $pos, join(" ", @features)), "\n";
    }
}

sub parse_to_features {
    my $parse = shift;
    my $line = shift;
    my @features;
    my %features;
    foreach my $val (@$parse) {
        if(exists($map{$val})) {
            my $name = $map{$val};
            $features{$name} = $val;
        }
        else {
            print STDERR "Unknown feature $val : $line\n";
        }
    }
    
    if(exists($features{pos})) {
        push(@features, $features{pos});
        delete $features{pos};
    }
    elsif(exists($features{depreciativity})) {
        push(@features, $features{depreciativity});
        delete $features{depreciativity};
    }
    elsif(exists($features{"winien-like"})) {
        push(@features, $features{"winien-like"});
        delete $features{"winien-like"};
    }
    elsif(exists($features{tense})) {
        push(@features, "verb");
    }
    elsif(exists($features{mode})) {
        push(@features, "verb");
    }
    else {
        print STDERR $line, "\n";
        print STDERR Dumper($parse);
    }
    
    foreach my $key (sort keys %features) {
        push(@features, "$key=$features{$key}")
    }
    
    return @features;
}

sub parse {
    my @results;
    my @tags = split(/[\+\|]/, shift);
    
    my @parses;
    
    foreach my $tag (@tags) {
        my @features = map { [split(/\./, $_)] } split(/:/, $tag);
        my @multiplied = multiply(\@features);
        
        push(@parses, @multiplied);
    }
    return @parses;
}

sub multiply {
    my $features = shift;
    my @tab = [];
    my @new_tab;
    foreach my $feat (@$features) {
        @new_tab = ();        
        foreach my $val (@$feat) {
            foreach my $subtab (@tab) {
                my @temptab = @$subtab;
                push(@temptab, $val);
                push(@new_tab, \@temptab);
            }
        }
        @tab = @new_tab;
        
    }
    return @tab;
}

__DATA__
adj	pos
aglt	pos
adjc	pos
adja	pos
adjp	pos
adv	pos
num	pos
ppron12	pos
ppron3	pos
pred	pos
prep	pos
siebie	pos
subst	pos
verb	pos
conj	pos
qub	pos
burk	pos
interj	pos
interp	pos
xxx	pos
brev	pos
nie	pos
ign	pos
comp	pos
nstd	pos
sg	number
pl	number
pltant	number
nom	case
gen	case
acc	case
dat	case
inst	case
loc	case
voc	case
pos	degree
com	degree
sup	degree
m	gender
m1	gender
m2	gender
m3	gender
m4	gender
n	gender
f	gender
n1	gender
n2	gender
p1	gender
p2	gender
p3	gender
pri	person
sec	person
ter	person
depr	depreciativity
winien	winien-like
aff	negation
neg	negation
perf	aspect
imperf	aspect
?perf	aspect
nakc	accentability
akc	accentability
praep	post-prepositionality
npraep	post-prepositionality
ger	tense
imps	tense
inf	tense
fin	tense
bedzie	tense
praet	tense
refl	tense
pact	tense
pant	tense
pcon	tense
ppas	tense
impt	mode
pot	mode
indecl	uninflected
irreg	irregularity
pun	fullstoppedness
npun	fullstoppedness
wok	vocalicity
nwok	vocalicity
agl	agglutination
nagl	agglutination
congr	accommodability
rec	accommodability
_	unknown;