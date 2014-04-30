# Scanner finite automaton configuration
# Syntax:
#   :start_state
#   @transition_state1                    characters_for_transition
#   @transition_state2                    characters_for_transition
#   ...
#   @transition_stateN                    characters_for_transition
# 
#   :[%|"|/]state1              [lexeme_identifier (positive integer)]
#   @transition_state           [characters_for_transition]
#   [@transition_state          [characters_for_transition] ]
#
#   ...
#   :final_state
#
#   %list of keywords
#
# Keywords will be assigned an identifier starting from 1 in accending order as they are defined.
# The identifiers for the non keyword lexemes have to be higher than the number of keywords.
# If there is a clash in identifiers, the behaviour is undefined
#
# State name prefixes:
#   % triggers a keyword lookup for lexemes of that state
#   " string literal - spaces will be consumed by the same state and appended to the lexeme
#   / end of line comment. Anything till the end of line will be discarded
#
# A @ transition without any characters is treated as a wildcard transition - if no characters from other transitions match,
# any character will trigger a transition to that state

:start
@identifier     abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_
@int_constant   0123456789 
@literal_start  '
@string_start   " 
@delimeter      ,
@semicolon      ;
@opening_brace  {
@closing_brace  }
@opening_brack  [
@closing_brack  ]
@opening_paren  (
@closing_paren  )
@star           *
@modulo         %
@slash          /
@hyphen         -
@plus           +
@equals         =
@bin_and        &
@bin_xor        ^
@bin_or         |
@not            !
@less_than      <
@more_than      >
@start

:literal_start
@literal_esc    \
@literal_end    abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789

:literal_esc
@literal_end    abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789'\

:literal_end        
@literal        '

:"string_start
@string_esc     \
@string_start   abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789*/%-+=&^|!<>;{}()[],'
@string         "

:"string_esc
@string_start   abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789*/%-+=&^|!<>;{}()[],'\

:operator
@operator       =-+&|<>
@fin

:%identifier    25
@identifier     abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789 
@fin

:int_constant   38
@int_constant   0123456789
@float_constant .
@fin

:float_constant 64
@float_constant 0123456789
@fin

:literal        39
@fin

:string         40
@fin

:delimeter      28
@fin

:opening_paren  26
@fin

:closing_paren  27
@fin

:star           24
@star_eq        =
@fin

:star_eq        47
@fin

:modulo         62
@mod_eq         =
@fin

:mod_eq         49
@fin

:assignment_op  200
@fin            

:slash          61
@eol_comment    /
@comment        *
@slash_eq       =
@fin

:slash_eq       48
@fin

:comment
@comment_star   *
@comment

:comment_star
@comment_end    /
@comment_star   *
@comment    

:comment_end
@fin

:/eol_comment
@fin

:hyphen         60
@minus_eq       =
@decrement      -
@fin

:minus_eq       46
@fin

:decrement      37
@fin

:plus           44
@plus_eq        =
@increment      +
@fin

:plus_eq        45
@fin

:increment      36
@fin

:equals         21
@eq_test        =
@fin

:bin_and        35
@and_eq         =
@logical_and    &
@fin

:and_eq         50
@fin

:logical_and    32
@fin

:bin_xor        34
@xor_eq         =
@fin

:xor_eq         51
@fin

:bin_or         33
@or_eq          =
@logical_or     |
@fin

:or_eq          52
@fin

:logical_or     31
@fin

:not            63
@not_eq         =
@fin

:eq_test        41
@fin

:not_eq         55
@fin

:less_than      56
@lt_eq          =
@shift_left     <
@fin

:lt_eq          58
@fin

:more_than      42
@mt_eq          =
@shift_right    >
@fin

:mt_eq          57
@fin

:shift_left     59
@sl_eq          =
@fin

:sl_eq          53
@fin

:shift_right    43
@sr_eq          =
@fin

:sr_eq          54
@fin

:semicolon      20
@fin

:opening_brace  22
@fin

:closing_brace  23
@fin

:opening_brack  29
@fin

:closing_brack  30
@fin

:fin

%int char void float
%if else
%while for continue break return
%input output

