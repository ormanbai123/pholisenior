// Language with tricky lookaheads:
%startsymbol S EOF
// Start symbol with end marker.
%rules
S =>
A =>
B =>
c b c a | A a A b ;
B ;
c ;

