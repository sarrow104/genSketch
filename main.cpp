#include <sss/path/glob_path.hpp>
#include <sss/path/glob_path_recursive.hpp>
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

#include <sss/util/PostionThrow.hpp>

#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <iostream>

#define VALUE_MSG(a) (#a) << " = " << (a)

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
        << "NOTE use file " << tpl_conf_name << ", " << tpl_order_name << " under xxx.tpl directory wisely!"
        << std::endl;
}

void list_cmd(const std::string& tmpl_dir)
{
    sss::path::file_descriptor fd;
    sss::path::glob_path_recursive gp(tmpl_dir, fd);
    while (gp.fetch()) {
        if (fd.is_normal_dir() && sss::is_end_with(fd.get_name(), ".tpl")) {
            std::cout << sss::path::no_suffix(sss::path::relative_to(fd.get_path(), tmpl_dir)) << std::endl;
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
    std::string full {tmpl_dir};
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
        // fall-throw:

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
    std::string full {tmpl_dir};
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
        std::cout << VALUE_MSG(order_file_path) << std::endl;
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
                rel_path = sss::path::simplify_copy(reg_order.submatch(2));
            }
            else if (reg_name.match(line)) {
                rel_path = sss::path::simplify_copy(reg_order.submatch(1));
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

void load_conf_variables(const std::string& get_command, sss::PenvMgr2& env, std::string& before_script, std::string& after_script)
{
    std::string conf_path = get_command;
    sss::path::append(conf_path, tpl_conf_name);
    if (sss::path::filereadable(conf_path)) {
        std::cout << VALUE_MSG(conf_path) << std::endl;
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
    }
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
        std::cout << order << " " << msg_path << "/" << " <- `mkdir -p`" << std::endl;
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
        std::string dir = sss::path::getcwd();
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

        while (idx < argc) {
            if (sss::is_begin_with(argv[idx], "-D") && sss::is_has(argv[idx], "=")) {
                char * eq_pos = std::strchr(argv[idx] + 2, '=');
                std::string key(argv[idx] + 2, std::distance(argv[idx] + 2, eq_pos));
                std::string value(eq_pos + 1);
                cml_env_variables[key] = value;
                // env.set(key, value);
                ++idx;
            }
            else if (get_command.empty()) {
                get_command = argv[idx++];
                std::cout << VALUE_MSG(get_command) << std::endl;
            }
            else if (target.empty()) {
                target = argv[idx++];
                std::cout << VALUE_MSG(target) << std::endl;
            }
            else if (dir.empty()) {
                dir = sss::path::full_of_copy(argv[idx++]);
                std::cout << VALUE_MSG(dir) << std::endl;
            }
        }

        if (get_command.empty()) {
            help_msg();
            return EXIT_SUCCESS;
        }

        if (target.empty()) {
            target = '.';
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

        std::cout << VALUE_MSG(get_command) << std::endl;

        std::vector<std::pair<std::string, order_t> > sorted_tpl_path;
        load_sorted_templates(get_command, sorted_tpl_path);
        std::string before_script;
        std::string after_script;
        load_conf_variables(get_command, env, before_script, after_script);
        for (const auto & i : cml_env_variables) {
            env.set(i.first, i.second);
        }

        // NOTE before-script 和 after-script的执行；
        // 脚本文本中，我也允许含有$变量；
        // 问题来了。如何同时处理PenvMgr2变量，以及os的环境变量呢？
        // 直接get_expr()，肯定不行；虽然PenvMgr2可以调用os.env，但是需要特定的语法。
        // $s.env_name；所以，唯一可行的办法是，将其包装为${t.(``)}变量，再get_expr()；
        // 同时，忽略其执行结果。
        gensketch_execute_script(env, before_script);

        for (const auto & i : sorted_tpl_path) {
            std::string out_path = sss::path::append_copy(dir,
                                                          env.get_expr(i.first));

            std::string target_rel_path = sss::path::relative_to(out_path, dir);

            if (*i.first.rbegin() == sss::path::sp_char) {
                gensketch_mkpath(out_path, target_rel_path, i.second);
                continue;
            }
            std::string content;
            bool using_template = false;
            if (sss::path::suffix(out_path) == ".tpl") {
                using_template = true;
                out_path = sss::path::no_suffix(out_path);
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

            std::string tpl_path = sss::path::append_copy(get_command, i.first);
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
            sss::path::chmod(out_path, sss::path::getmod(tpl_path));
        }

        gensketch_execute_script(env, after_script);

        return EXIT_SUCCESS;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "unexcept exception! encountered!" << std::endl;
    }
}
