#include <sss/path/glob_path.hpp>
#include <sss/path/glob_path_recursive.hpp>
#include <sss/bit_operation/bit_operation.h>
#include <sss/utlstring.hpp>
#include <sss/ps.hpp>
#include <sss/path.hpp>
#include <sss/fs.hpp>
#include <sss/penvmgr2.hpp>
#include <sss/time.hpp>
#include <sss/log.hpp>
#include <sss/Terminal.hpp>
#include <sss/regex/cregex.hpp>
#include <sss/dosini/dosini.hpp>
#include <sss/environ.hpp>
#include <sss/config.hpp>
#include <sss/debug/value_msg.hpp>

#include <sss/util/PostionThrow.hpp>
#include <sss/cygpath.hpp>

#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <iostream>

bool is_fake_shell()
{
#ifdef __WIN32__
    static bool is_fake = sss::is_equal(sss::env::get("TERM"), std::string("xterm"));
    return is_fake;
#else
    return false;
#endif
}

void path_win2mixed_if(std::string& path)
{
    if (::is_fake_shell()) {
        sss::replace_all(path, "\\", "/");
    }
}

void path_mixed2win_if(std::string& path)
{
    if (::is_fake_shell()) {
        sss::replace_all(path, "/", "\\");
    }
}

const std::string tpl_help_name = ".tpl.help.md";
const std::string tpl_conf_name = ".tpl.config.ini";
const std::string tpl_order_name = ".tpl.order.txt";

class order_t {
    uint32_t m_high;
    uint32_t m_low;
public:
    order_t() : m_high(0), m_low(0)
    { }
    explicit order_t(uint32_t high) : m_high(high), m_low(0)
    {}
    order_t(uint32_t high, uint32_t low) : m_high(high), m_low(low)
    {}

    uint32_t high() const {
        return m_high;
    }
    uint32_t low() const {
        return m_low;
    }
    bool operator < (const order_t& rhs) const {
        return m_high < rhs.m_high || (m_high == rhs.m_high && m_low < rhs.m_low);
    }

    bool operator == (const order_t& rhs) const {
        return m_high == rhs.m_high && m_low == rhs.m_low;
    }
};

inline std::ostream& operator << (std::ostream& o, const order_t& order) {
    o << "(" << order.high() << ", " << order.low() << ")";
    return o;
}

void help_msg()
{
    std::string app = sss::path::basename(sss::path::getbin());
    std::cout
        << app << " --help\n"
        << "\t print this message\n"
        << std::endl;

    std::cout
        << app << " --help get_command\n"
        << "\t print the content of file " << tpl_help_name << " under xxx.tpl directory.\n"
        << std::endl;

    std::cout
        << app << " --list\n"
        << "\t list all valid template get-command\n"
        << std::endl;

    std::cout
        << app << " --edit command\n"
        << "\t return the fullpath to specific command template path\n"
        << std::endl;

    std::cout
        << app << " get-command [Target-name] [out-directory] [-Denvirable=value]*\n"
        << "\t use template project <get-command>, with <target-name>,\n"
        << "\t apply in dir <out-directory>\n"
        << "\t you can add any numbers of -Dxxx=yyyy style parameters to shell script\n"
        << std::endl;

    std::cout << std::string(80, '=') << "\n" << std::endl;

    std::cout
        << tpl_conf_name
        << "\t a dosini config file for specific template setting\n"
        << "\n"
        << "\t [env-variable]\n"
        << "\t\t template author may put default PenvMgr variable assignment here;\n"
        << "\t\t one line per setting\n"
        << "\n"
        << "\t [scripts]\n"
        << "\t\t template user may specific shell script calling schema here\n"
        << "\t\t before-script for `before` trigger\n"
        << "\t\t after-script for `after` trigger\n"
        << "\n"
        << "\t [default-template]\n"
        << "\t\t only one predefined variable `fname` for editer like vim to edit with\n"
        << "\t\t after the current template task has been finished\n"
        << "\n"
        << std::endl;

    std::cout << std::string(80, '-') << "\n" << std::endl;

    std::cout
        << tpl_order_name
        << "\t specific the template file action appling order\n"
        << "\n"
        << "\t> [order-number] template-file-name\n"
        << "\t\t NOTE if no file list in this file, then all template file, \n"
        << "\t\t will be apply as the order it's iterator order by file-system.\n"
        << "\t\t NOTE\n"
        << "\t\t 1. the default priority order-number is 50\n"
        << "\t\t 2. the less order-number, the earlier will by apply to create\n"
        << "\n"
        << std::endl;

    std::cout << std::string(80, '-') << "\n" << std::endl;

    std::cout
        << tpl_help_name
        << "\t specific the template setting user decription\n"
        << "\n"
        << "\t\t NOTE this is just a markdown file\n"
        << "\n"
        << std::endl;

    std::cout << std::string(80, '=') << "\n" << std::endl;

    std::cout
        << "NOTE use file " << tpl_conf_name << ", " << tpl_order_name << ", " << tpl_help_name << " under xxx.tpl directory wisely!"
        << std::endl;
}

void list_cmd(const std::string& tmpl_dir)
{
    sss::path::file_descriptor fd;
    sss::path::glob_path_recursive gp(tmpl_dir, fd);
    while (gp.fetch()) {
        if (fd.is_normal_dir() && sss::is_end_with(fd.get_name(), ".tpl")) {
            std::string command = sss::path::no_suffix(sss::path::relative_to(fd.get_path(), tmpl_dir));
            path_win2mixed_if(command);
            std::cout << command << std::endl;
            gp.jump();
        }
    }
}

void genTplFile(const std::string& bin_dir, const std::string& name, const std::string& out_dir)
{
    std::string src_path = sss::path::append_copy(bin_dir, name);
    std::string tar_path = sss::path::append_copy(out_dir, name);
    if (sss::path::filereadable(src_path)) {
        sss::path::copy(src_path, tar_path);
    }
    else {
        sss::fs::touch(tar_path);
    }
}

void edit_tpl(const std::string& tmpl_dir, const std::string& command)
{
    std::string full(tmpl_dir);
    sss::path::append(full, command + ".tpl");
    switch (sss::path::file_exists(full)) {
    case sss::PATH_NOT_EXIST:
        sss::path::mkpath(full);
        {
            std::string bin_dir = sss::path::dirname(sss::path::getbin());
            genTplFile(bin_dir, tpl_conf_name, full);
            genTplFile(bin_dir, tpl_help_name, full);
            genTplFile(bin_dir, tpl_order_name, full);
        }
        SSS_FALLTHROUGH;

    case sss::PATH_TO_DIRECTORY:
        std::cout << full;
        break;

    case sss::PATH_TO_FILE:
        // NOTE something wrong here!
        break;
    }
}

void edit_tpl(const std::string& tmpl_dir, const std::string& command, const std::string& refer_command)
{
    std::string tar = sss::path::append_copy(tmpl_dir, command + ".tpl");
    std::string src = sss::path::append_copy(tmpl_dir, refer_command + ".tpl");

    if (sss::path::file_exists(tar) != sss::PATH_NOT_EXIST) {
        if (sss::path::file_exists(tar) == sss::PATH_TO_DIRECTORY) {
            std::cout << tar;
            return;
        }
        std::cout
            << command << " already exists!"
            << std::endl;
        return;
    }
    if (sss::path::file_exists(src) != sss::PATH_TO_DIRECTORY) {
        std::cout
            << refer_command << " not exists!"
            << std::endl;
        return;
    }
    if (sss::path::file_exists(sss::path::dirname(tar)) == sss::PATH_NOT_EXIST) {
        sss::path::mkpath(sss::path::dirname(tar));
    }
    if (sss::path::file_exists(sss::path::dirname(tar)) == sss::PATH_TO_DIRECTORY) {
        sss::path::copy_dir(src, tar);
    }
    else {
        std::cout
            << command << " can not be created!"
            << std::endl;
        return;
    }
}

void help_tpl(const std::string& tmpl_dir, const std::string& command)
{
    std::string full (tmpl_dir);
    sss::path::append(full, command + ".tpl");
    if (sss::path::file_exists(full) != sss::PATH_TO_DIRECTORY) {
        std::cout
            << sss::Terminal::error
            << "# ERROR #\n"
            << "command `" << command << "` not exists!"
            << sss::Terminal::end
            << std::endl;
        return;
    }
    sss::path::append(full, tpl_help_name);
    if (sss::path::filereadable(full)) {
        std::string content;
        sss::path::file2string(full, content);
        std::cout
            << content
            << std::endl;
    }
    else {
        std::cout
            << sss::Terminal::error
            << "# ERROR #\n"
            << "no help msg for `" << command << "`"
            << sss::Terminal::end
            << std::endl;
    }
}

// std::vector<std::pair<std::string, order_t> > sorted_tpl_path;
void load_sorted_templates(const std::string& get_command,
                           std::vector<std::pair<std::string, order_t> >& sorted_tpl_path)
{
    // TODO
    // read_config!
    // 1. 将.tpl所有文件，检索出来，并导出为外部序列；
    // 2. 应用 .tpl.config 的优先级描述；对序列重新排序；
    // 3. 逐个使用模板规则，创建文件；
    std::map<std::string, order_t> tpl_files;
    {
        sss::path::file_descriptor fd;
        sss::path::glob_path_recursive gp(get_command, fd);
        uint32_t low_order = 0;
        while (gp.fetch()) {
            std::string rel_path;
            if (fd.is_normal_file()) {
                if (fd.get_name() == tpl_conf_name ||
                    fd.get_name() == tpl_help_name ||
                    fd.get_name() == tpl_order_name)
                {
                    continue;
                }
                rel_path = sss::path::relative_to(fd.get_path(), get_command);
            }
            else if (fd.is_normal_dir()) {
                rel_path = sss::path::relative_to(fd.get_path(), get_command) + sss::path::sp_char;
            }
            else {
                continue;
            }
            // 默认优先级
            tpl_files[rel_path] = order_t(50, low_order++);
        }
    }

    std::string order_file_path = sss::path::append_copy(get_command, tpl_order_name);
    if (sss::path::filereadable(order_file_path)) {
        std::cout << SSS_VALUE_MSG(order_file_path) << std::endl;
        std::ifstream order_file(order_file_path);
        std::string line;
        sss::regex::CRegex reg_order(R"(^0*([0-9]+)\s+(.+)$)");
        // NOTE FIXME 如果文件名，就是数字，如何处理？
        sss::regex::CRegex reg_name(R"(^\s*(.+)\s*$)");
        uint32_t low_order = 0;
        while (std::getline(order_file, line)) {
            uint32_t high_order = 50;
            std::string rel_path;
            if (reg_order.match(line)) {
                high_order = sss::string_cast<int>(reg_order.submatch(1));
                rel_path = reg_order.submatch(2);
                path_mixed2win_if(rel_path);
                rel_path = sss::path::simplify(rel_path);
            }
            else if (reg_name.match(line)) {
                rel_path = reg_order.submatch(1);
                path_mixed2win_if(rel_path);
                sss::path::simplify(rel_path);
            }
            else {
                continue;
            }
            order_t order = order_t(high_order, low_order++);
            // std::cout << order << " = " << rel_path << std::endl;
            if (tpl_files.find(rel_path) == tpl_files.end()) {
                std::cout
                    << sss::Terminal::error
                    << "order setting file\n"
                    << "\t" << rel_path << " is not exists!"
                    << sss::Terminal::end
                    << std::endl;
                continue;
            }
            tpl_files[rel_path] = order;
        }
    }

    std::vector<std::pair<std::string, order_t> > sorted_tpl_path_tmp;
    std::copy(tpl_files.begin(), tpl_files.end(), std::back_inserter(sorted_tpl_path_tmp));
    std::sort(sorted_tpl_path_tmp.begin(), sorted_tpl_path_tmp.end(),
              [=](const std::pair<std::string, order_t>& lhs,
                  const std::pair<std::string, order_t>& rhs)->bool {
                  return lhs.second < rhs.second;
              });
    std::swap(sorted_tpl_path_tmp, sorted_tpl_path);
}

void load_conf_variables(const std::string& get_command,
                         sss::PenvMgr2& env,
                         std::string& before_script,
                         std::string& after_script,
                         std::string& default_template)

{
    std::string conf_path = get_command;
    sss::path::append(conf_path, tpl_conf_name);
    if (!sss::path::filereadable(conf_path)) {
        std::cout << "read conf file faild: `" << conf_path << "`" << std::endl;
        return;
    }

    std::cout << SSS_VALUE_MSG(conf_path) << std::endl;
    sss::dosini penvIni(conf_path);

    // 同名变量，只能有唯一定义式；
    // 如果ini配置中，出现同名定义式，以最后一个出现的定义式，为该名称变量的定义！
    // 这是可以由sss::dosini的顺序解析来决定的！
    const sss::dosini::section_t& sec_var = penvIni.section("env-variable");
    for (const auto & item : sec_var) {
        std::cout
            << item.first << "=" << item.second.get()
            << std::endl;
        env.set(item.first, item.second.get());
    }
    std::cout << std::endl;
    penvIni.get("scripts", "before-script", before_script);
    penvIni.get("scripts", "after-script", after_script);
    penvIni.get("default-template", "fname", default_template);
}

void gensketch_execute_script(sss::PenvMgr2& env, const std::string& script)
{
    if (script.empty()) {
        return;
    }
    (void)env.get_expr("${t.(`" + script + "`)}");
}

void gensketch_mkpath(const std::string& out_path, const std::string& msg_path, const order_t& order)
{
    if (sss::path::file_exists(out_path) == sss::PATH_NOT_EXIST) {
        if (!sss::path::mkpath(out_path)) {
            std::ostringstream oss;
            oss << "failed mkdir " << out_path;
            throw std::runtime_error(oss.str());
        }
        std::cout << order << " " << msg_path << sss::path::sp_char << " <- `mkdir -p`" << std::endl;
    }
}

int main (int argc, char *argv[])
{
    try {
        (void) argc;
        (void) argv;

        sss::PenvMgr2 env;
        env.set("tmpl_dir",
                sss::path::append_copy(sss::path::dirname(sss::path::getbin()),
                                       "template"));

        std::string get_command;
        std::string target;
        std::string dir;
        std::map<std::string, std::string> cml_env_variables;

        int idx = 1;

        if (idx < argc) {
            if (argv[idx] == std::string("--help")) {
                ++idx;
                if (idx < argc) {
                    help_tpl(env.get("tmpl_dir"), argv[idx]);
                }
                else {
                    help_msg();
                }
                return EXIT_SUCCESS;
            }
            else if (argv[idx] == std::string("--list")) {
                list_cmd(env.get("tmpl_dir"));
                return EXIT_SUCCESS;
            }
            else if (argv[idx] == std::string("--edit")) {
                if (idx + 2 < argc) {
                    edit_tpl(env.get("tmpl_dir"), argv[idx + 1], argv[idx + 2]);
                    return EXIT_SUCCESS;
                }
                else if (idx + 1 < argc) {
                    edit_tpl(env.get("tmpl_dir"), argv[idx + 1]);
                    return EXIT_SUCCESS;
                }
            }
        }

        std::vector<std::string> args4Penv;

        while (idx < argc) {
            if (sss::is_begin_with(argv[idx], "-D") && sss::is_has(argv[idx], "=")) {
                char * eq_pos = std::strchr(argv[idx] + 2, '=');
                std::string key(argv[idx] + 2, std::distance(argv[idx] + 2, eq_pos));
                std::string value(eq_pos + 1);
                cml_env_variables[key] = value;
                ++idx;
                continue;
            }

            if (get_command.empty()) {
                get_command = argv[idx];
                std::cout << SSS_VALUE_MSG(get_command) << std::endl;
            }
            else if (target.empty()) {
                target = argv[idx];
                std::cout << SSS_VALUE_MSG(target) << std::endl;
            }
            else if (dir.empty()) {
                dir = sss::path::full_of_copy(argv[idx]);
                std::cout << SSS_VALUE_MSG(dir) << std::endl;
            }
            args4Penv.push_back(argv[idx++]);
        }

        if (dir.empty()) {
            dir = sss::path::getcwd();
        }

        env.set("ARG_COUNT", sss::cast_string(args4Penv.size()));
        for (size_t i = 0; i < args4Penv.size(); ++i) {
            env.set("ARGV_" + sss::cast_string(i), args4Penv[i]);
        }

        if (get_command.empty()) {
            help_msg();
            return EXIT_SUCCESS;
        }

        if (target.empty()) {
            target = ".";
        }
        if (target == ".") {
            target = sss::path::basename(sss::path::getcwd());
            std::cout << "using current working dir" << std::endl;
        }
        get_command = sss::path::append_copy(env.get("tmpl_dir"), get_command + ".tpl");
        if (sss::path::file_exists(get_command) != sss::PATH_TO_DIRECTORY) {
            std::cerr << get_command << " not exists!" << std::endl;
            return EXIT_FAILURE;
        }

        env.set("Target", target);
        env.set("timestamp", sss::time::strftime("%Y%m%d"));

        std::cout << SSS_VALUE_MSG(get_command) << std::endl;

        std::vector<std::pair<std::string, order_t> > sorted_tpl_path;
        load_sorted_templates(get_command, sorted_tpl_path);
        std::string before_script;
        std::string after_script;
        std::string default_template; // NOTE 应用template工程之后，在vim中应该预览/编辑的文件，所对应的模板文件路径。
        load_conf_variables(get_command, env, before_script, after_script, default_template);
        for (const auto & i : cml_env_variables) {
            env.set(i.first, i.second);
        }
        if (!before_script.empty()) {
            std::cout << SSS_VALUE_MSG(before_script) << std::endl;
        }
        if (!after_script.empty()) {
            std::cout << SSS_VALUE_MSG(after_script) << std::endl;
        }
        if (!default_template.empty()) {
            std::cout << SSS_VALUE_MSG(default_template) << std::endl;
        }
        if (!before_script.empty() || !after_script.empty() || !default_template.empty()) {
            std::cout << std::endl;
        }

        // NOTE before-script 和 after-script的执行；
        // 脚本文本中，我也允许含有$变量；
        // 问题来了。如何同时处理PenvMgr2变量，以及os的环境变量呢？
        // 直接get_expr()，肯定不行；虽然PenvMgr2可以调用os.env，但是需要特定的语法。
        // $s.env_name；所以，唯一可行的办法是，将其包装为${t.(``)}变量，再get_expr()；
        // 同时，忽略其执行结果。
        gensketch_execute_script(env, before_script);
        std::string default_file; // NOTE 应用template工程之后，在vim中应该预览/编辑的文件路径。

        for (const auto & i : sorted_tpl_path) {
            std::string out_path = sss::path::append_copy(dir,
                                                          env.get_expr(i.first));

            std::string target_rel_path = sss::path::relative_to(out_path, dir);

            if (sss::path::is_end_with_spchar(i.first)) {
                gensketch_mkpath(out_path, target_rel_path, i.second);
                continue;
            }
            std::string content;
            bool using_template = false;
            if (sss::path::suffix(out_path) == ".tpl") {
                using_template = true;
                out_path = sss::path::no_suffix(out_path);
            }

            if (default_template.empty() && default_file.empty()) {
                default_file = out_path;
            }
            std::string tpl_path = sss::path::append_copy(get_command, i.first);
            //std::cout << SSS_VALUE_MSG(tpl_path) << std::endl;

            if (!default_template.empty()) {
                if (tpl_path == sss::path::append_copy(get_command, default_template)) {
                    default_file = out_path;
                }
            }

            // NOTE TODO
            // 对已经存在的，做如何处理？
            // 应该再增加一个force_rewrite 参数；
            // 同时，在rewrite之前，backup一下。
            if (sss::path::file_exists(out_path)) {
                std::cerr << out_path << " already exists!" << std::endl;
                continue;
            }

            gensketch_mkpath(sss::path::dirname(out_path), sss::path::dirname(target_rel_path), i.second);

            std::cout << i.second << " " << target_rel_path << " <- " << (using_template ? "template" : "copy") << std::endl;

            sss::path::file2string(tpl_path, content);
            std::ofstream ofs(out_path.c_str(), std::ios_base::out | std::ios_base::binary);
            if (!ofs.good()) {
                std::ostringstream oss;
                oss << "failed open file `" << out_path << "` to write";
                throw std::runtime_error(oss.str());
            }
            if (using_template) {
                env.set_shellscript_workdir(sss::path::dirname(out_path));
                ofs << env.get_expr(content);
                env.unset_shellscript_workdir();
            }
            else {
                ofs << content;
            }
#ifndef __WIN32__
            sss::path::chmod(out_path, sss::path::getmod(tpl_path));
#endif
        }

        gensketch_execute_script(env, after_script);

        if (!default_file.empty()) {
            std::cout << "default-file=" << default_file << std::endl;
        }

        return EXIT_SUCCESS;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "unexcept exception! encountered!" << std::endl;
    }
}
