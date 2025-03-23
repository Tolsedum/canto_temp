#include "canto_temp/parser_logic/Token.hpp"

void canto_temp::Token::initToken(char current, char next){
    switch (current) {
        case '+':{
            cell_ = Token::Cell::plus;
            break;
          }
        case '-':{
            cell_ = Token::Cell::minus;
            break;
          }
        case '*':{
            cell_ = Token::Cell::times;
            break;
          }
        case '/':{
            cell_ = Token::Cell::slash;
            break;
          }
        case '^':{
            cell_ = Token::Cell::power;
            break;
          }
        case '%':{
            cell_ = Token::Cell::percent;
            break;
          }
        case '.':{
            cell_ = Token::Cell::dot;
            break;
          }
        case ',':{
            cell_ = Token::Cell::comma;
            break;
          }
        case ':':{
            cell_ = Token::Cell::colon;
            break;
          }
        case '(':{
            cell_ = Token::Cell::left_paren;
            break;
          }
        case ')':{
            cell_ = Token::Cell::right_paren;
            break;
          }
        case '[':{
            cell_ = Token::Cell::left_bracket;
            break;
          }
        case ']':{
            cell_ = Token::Cell::right_bracket;
            break;
          }
        case '{':{
            cell_ = Token::Cell::left_brace;
            break;
          }
        case '}':{
            cell_ = Token::Cell::right_brace;
            break;
          }
        case '>':{
            if (next == '=') {
                cell_ = Token::Cell::greater_equal;
            }else{
                cell_ = Token::Cell::greater_than;
            }
            break;
        }
        case '<':{
            if (next == '=') {
                cell_ = Token::Cell::less_equal;
            }else{
                cell_ = Token::Cell::less_than;
            }
            break;
        }
        case '=':{
            if (next == '=') {
                cell_ = Token::Cell::equal;
            }else{
                cell_ = Token::Cell::unknown;
            }
            break;
        }
        case '!':{
            if (next == '=') {
                cell_ = Token::Cell::not_equal;
            }else{
                cell_ = Token::Cell::unknown;
            }
            break;
        }
        case '\"':{
            cell_ = Token::Cell::string;
            break;
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':{
            cell_ = Token::Cell::number;
            break;
        }
        case '_':
        case '@':
        case '$':{
            //   return scan_id();
            cell_ = Token::Cell::unknown;
            break;
        }
        default:
          cell_ = Token::Cell::unknown;
        }
}