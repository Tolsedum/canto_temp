#include "canto_temp/parser_logic/tools.hpp"

/** -------------------- Filters -------------------- */

void canto_temp::parser_logic::upper(
    nlohmann::json& dict,
    nlohmann::json
){
    if(dict.is_string()){
        std::string str = dict;
        std::transform(
            str.begin(), 
            str.end(), 
            str.begin(), 
            ::toupper
        );
        dict = str;
    }
}

void canto_temp::parser_logic::lower(
    nlohmann::json& dict,
    nlohmann::json
){
    if(dict.is_string()){
        std::string str = dict;
        std::transform(
            str.begin(), 
            str.end(), 
            str.begin(), 
            ::tolower
        );
        dict = str;
    }
}

void canto_temp::parser_logic::abs_(
    nlohmann::json& dict,
    nlohmann::json
){
    if(dict.is_number_unsigned()){
        dict = abs((unsigned)dict);
    }else if(dict.is_number_integer()){
        dict = abs((int)dict);
    }else if(dict.is_number_float()){
        dict = abs((float)dict);
    }
}

void canto_temp::parser_logic::batch(
    nlohmann::json& dict, 
    nlohmann::json params
){
    if(dict.is_array()){
        nlohmann::json tmp_dict;
        std::size_t iter = 0;
        int iter_key = 0;
        int block_iter = 0;
        int count_in_block = params[0];
        std::string default_value = params[1];
        bool flash_iter_key = true;
        if(params.size() == 3 
            && !(bool)params[2]
        ){
            flash_iter_key = false;
        }
        while (true){
            if(count_in_block > 0){
                for (int i = 0; i < count_in_block; i++){
                    if(iter < dict.size()){
                        tmp_dict[block_iter][iter_key] = 
                            dict[iter];
                    }else{
                        tmp_dict[block_iter][iter_key] = 
                            default_value;
                    }
                    iter++;
                    iter_key++;
                }
                if(!flash_iter_key){
                    iter_key = 0;
                }
            }else {
                break;
            }
            
            block_iter++;
            if(dict.size() <= iter){
                break;
            }
        }
        dict = tmp_dict;
    }
}

void canto_temp::parser_logic::capitalize(
    nlohmann::json& dict,
    nlohmann::json
){
    if(dict.is_string()){
        std::string str{dict.get<std::string>()};
        str[0] = std::toupper(str[0]);
        dict = str;
    }
}

void canto_temp::parser_logic::column(
    nlohmann::json& dict, 
    nlohmann::json params
){
    std::vector<nlohmann::json> list{};
    nlohmann::json key = params[0];
    if(dict.is_array()){
        for (auto &&iter : dict.items()){
            for (auto &&i : iter.value().items()){
                if(key == i.key()){
                    list.push_back(i.value());
                }
            }
        }
    }
    dict = list;
}

void canto_temp::parser_logic::country_name(
    nlohmann::json& dict, nlohmann::json params
){
    static std::map<std::string, std::map<std::string, std::string>> 
        country_name_list;
    std::string locale_;
    locale_ = params[0];
    if(!locale_.empty()){
        dict = getLenContent(
            locale_,
            "countrys/" + locale_ + "/country_name.len",
            dict.get<std::string>(),
            country_name_list
        );
    }
}

void canto_temp::parser_logic::currency_name(
    nlohmann::json& dict, nlohmann::json params
){
    static std::map<std::string, std::map<std::string, std::string>> 
        currency_name_list;
    std::string locale_;
    locale_ = params[0];
    if(!locale_.empty()){
        dict = getLenContent(
            locale_,
            "currency/" + locale_ + "/currency_name.len",
            dict.get<std::string>(),
            currency_name_list
        );
    }
}

void canto_temp::parser_logic::default_(
    nlohmann::json& dict, nlohmann::json params
){
    if(dict.empty()){
        dict = params[0];
    }
}

void canto_temp::parser_logic::escape(
    nlohmann::json& dict, nlohmann::json
){
    // auto fn_js_escape = [](){

    // };
    auto fn_html_escape = [&dict](){
        std::string str = dict.get<std::string>();
        std::string buffer;
        buffer.reserve(str.size());
        for(size_t pos = 0; pos != str.size(); ++pos) {
            switch(str[pos]) {
                case '&':  buffer.append("&amp;");       break;
                case '\"': buffer.append("&quot;");      break;
                case '\'': buffer.append("&apos;");      break;
                case '<':  buffer.append("&lt;");        break;
                case '>':  buffer.append("&gt;");        break;
                default:   buffer.append(&str[pos], 1); break;
            }
            dict = str;
        }
    };

    // auto fn_css_escape = [](){
        
    // };
    fn_html_escape();
}

void canto_temp::parser_logic::first(
    nlohmann::json& dict, nlohmann::json
){
    if(dict.is_string()){
        std::string str = dict.get<std::string>();
        dict = str[0];
    }else if(dict.is_array()){
        for (auto &&iter : dict.items()){
            dict = iter.value();
            break;
        }
    }
}

void canto_temp::parser_logic::format(
    nlohmann::json& dict, nlohmann::json params
){
    std::string search{"%s"};
    std::string str{dict.get<std::string>()};
    std::size_t pos = 0;
     for(auto &&item : params.items()){
        pos = str.find(search, pos);
        if (pos == std::string::npos) 
            break;
        std::string replace_to{};
        if(!item.value().empty()){
            replace_to = item.value().get<std::string>();
        }
        str.replace(pos, search.length(), replace_to);
        pos += replace_to.length();
    }
    dict = str;
}

void canto_temp::parser_logic::join(
    nlohmann::json& dict, nlohmann::json params
){
    int iter = 0;
    std::string sep{}, ret_value{};
    for (auto &&item : dict.items()){
        if(!params.empty() && (int)params.size() > iter){
            sep = params[iter].get<std::string>();
        }
        ret_value.append(item.value()).append(sep);
        sep.clear();
        iter++;
    }
    dict = ret_value;
}

void canto_temp::parser_logic::json_encode(
    nlohmann::json& dict, nlohmann::json 
){
    dict = dict.dump();
}

void canto_temp::parser_logic::keys(
    nlohmann::json& dict, nlohmann::json 
){
    std::vector<nlohmann::json> ret_value;
    for (auto &&item : dict.items()){
        ret_value.push_back(item.key());
    }
    dict = ret_value;
}

void canto_temp::parser_logic::last(
    nlohmann::json& dict, nlohmann::json 
){
    if(dict.is_string()){
        std::string str{dict.get<std::string>()};
        dict = std::string(1, str[str.size()-1]);
    }else if(dict.is_array()){
        auto iter = dict.end()-1;
        dict = iter.value();
    }
}

void canto_temp::parser_logic::length(
    nlohmann::json& dict, nlohmann::json 
){
    std::size_t size = 0;
    if(dict.is_string()){
        size = dict.get<std::string>().size();
    }else{
        size = dict.size();
    }
    dict = size;
}

void canto_temp::parser_logic::merge(
    nlohmann::json& dict, nlohmann::json params
){
    std::vector<nlohmann::json> ret_value;

    for (auto &&iter : dict.items()){
        ret_value.push_back(iter.value());
    }
    
    for (auto &&iter : params.items()){
        for (auto &&i : iter.value().items()){
            ret_value.push_back(i.value());
        }
    }
    
    dict = ret_value;
}

void canto_temp::parser_logic::nl2br(
    nlohmann::json& dict, nlohmann::json
){
    if(dict.is_string()){
        std::string str{dict.get<std::string>()}, br{"<br/>"};
        std::size_t pos = 0;
        while (true){
            pos = str.find('\n', pos);
            if(pos == std::string::npos){
                break;
            }
            
            str.replace(pos, br.size(), br);
        }
        dict = str;
    }
}

void canto_temp::parser_logic::replace(
    nlohmann::json& dict, nlohmann::json params
){
    if(params.is_array() && params.size() > 0){
        std::string str{dict.get<std::string>()};
        for (auto &&item : params[0].items()){
            std::string search{item.key()}, rep{item.value()};
            std::size_t pos = str.find(search);
            if(pos != std::string::npos){
                str.replace(pos, search.size(), rep);
            }
        }
        dict = str;
    }
}

void canto_temp::parser_logic::reverse(
    nlohmann::json& dict, nlohmann::json
){
    if(dict.is_string()){
        std::string str{dict.get<std::string>()};
        std::string new_str{};
        for (size_t i = str.size(); i > 0; i--){
            new_str.append(1, str[i]);
        }
        dict = new_str;
    }else{
        nlohmann::json container;
        for (auto &&item : dict.items()){
            container[item.value()] = item.key();
        }
        dict = container;
    }
}

void canto_temp::parser_logic::round(
    nlohmann::json& dict, nlohmann::json
){
    if(dict.is_number()){
        double d = dict.get<double>();
        dict = std::round(d);
    }
}

void canto_temp::parser_logic::e(
    nlohmann::json& dict, nlohmann::json
){
    dict = entityParser(dict.get<std::string>());
}
/** ------------------- END Filters ------------------ */

 std::string canto_temp::parser_logic::entityParser(std::string text){
    std::unordered_map<std::string, char> convert({
        {"&quot;", '"'},
        {"&apos;", '\''},
        {"&amp;", '&'},
        {"&gt;", '>'},
        {"&lt;", '<'},
        {"&frasl;", '/'}
    });
    std::string res{}, tmp{};
    std::size_t amp_iter = std::string::npos;
    bool is_from_convert = false;
    bool append = true;
    for (std::size_t i = 0; i < text.size(); ++ i) {
        if(text[i] == '&'){
            amp_iter = i;
            tmp.append(1, text[i]);
            append = false;
        }else if(amp_iter != std::string::npos){
            tmp.append(1, text[i]);
            if(text[i] == ';'){
                is_from_convert = true;
            }
        }
        for (auto it = begin(convert); it != end(convert); ++ it) {
            if(is_from_convert){
                if(tmp == it->first){
                    is_from_convert = false;
                    amp_iter = std::string::npos;
                    append = true;
                    res.append(tmp);
                    break;
                }
            }else if(amp_iter == std::string::npos){
                if(text[i] == it->second){
                    res.append(it->first);
                    // append = false;
                    // break;
                    goto skip_append_char;
                }
            }else{
                break;
            }
        }
        if(is_from_convert){
            is_from_convert = false;
            amp_iter = std::string::npos;
            res.append(tmp);
        }else if (append) {
            if(tmp.size() > 0){
                tmp.clear();
            }else{
                res += text[i];
            }
        }
skip_append_char:
    }
    return res;
}

std::map<std::string, std::string> canto_temp::parser_logic::lenReade(
    std::string file_path
){
    std::map<std::string, std::string> ret_value;

    std::ifstream file_p(file_path);
    if(file_p.is_open()){
        auto fn_clear = [](std::string line, std::string code_word){
            std::string code{line.substr(code_word.size())};
            canto_temp::parser_logic::ltrim(code);
            canto_temp::parser_logic::rtrim(code);
            code.erase(code.size()-1);
            return code.substr(1);
        };
        
        std::string code_word{"code:"};
        std::string value_word{"len:"};
        std::string code_name, len_name;

        for (std::string line; std::getline(file_p, line); ){
            std::size_t pos = line.find(code_word);
            if(pos == std::string::npos){
                pos = line.find(value_word);
                if(pos != std::string::npos){
                    len_name = fn_clear(line, value_word);
                    ret_value[code_name] = len_name;
                }
            }else{
                code_name = fn_clear(line, code_word);
            }
        }
    }
    return ret_value;
}

std::string canto_temp::parser_logic::getLenContent(
        std::string locale_,
        std::string path,
        std::string lang_code,
        std::map<std::string, std::map<std::string, std::string>>& list
){
    std::string ret_value;
    auto iter = list.find(locale_);
    if(iter == list.end()){
        list[locale_] = lenReade(path);
        iter = list.find(locale_);
    }
    if(iter != list.end()){
        auto iter_name = iter->second.find(lang_code);
        if(iter_name != iter->second.end()){
            ret_value = iter_name->second;
        }else{
            ret_value = "";
        }
    }
        
    return ret_value;
}


void canto_temp::parser_logic::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void canto_temp::parser_logic::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

bool canto_temp::parser_logic::isEmptyDicVar(nlohmann::json &dict){
    return dict.is_null()
        ? true
        : (dict.is_boolean()
            ? !dict.get<bool>()
            : (dict.is_string() 
                ? std::string(dict).empty()
                : (dict.is_number() 
                    ? dict.get<float>() != 0
                    : dict.empty()
                )));
}

bool canto_temp::parser_logic::getBoolDicVar(nlohmann::json dic){
    return !isEmptyDicVar(dic);
}

bool canto_temp::parser_logic::compare(
    nlohmann::json dic, nlohmann::json dic1, char comp
){
    if(comp == '='){
        return dic == dic1;
    }else if(comp == '!'){
        return dic != dic1;
    }
    return false;
}
