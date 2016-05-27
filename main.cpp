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

#include <sss/util/PostionThrow.hpp>

#include <string>
#include <fstream>
#include <iostream>

#define NAME_WAPPER(a) (#a) << " = " << (a)

void help_msg()
{
    std::string app = sss::path::basename(sss::path::getbin());
    std::cout
        << app << " --help\n"
        << "\t print this message\n"
        << std::endl;

    std::cout
        << app << " --list\n"
        << "\t list all valid template get-command\n"
        << std::endl;

    std::cout
        << app << " --edit command"
        << "\t return the fullpath to specific command template path"
        << std::endl;

    std::cout
        << app << " get-command [Target-name] [out-directory]\n"
        << "\t use template project <get-command>, with <target-name>,\n"
        << "\t apply in dir <out-directory>\n"
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

void edit_tpl(const std::string& tmpl_dir, const std::string& command)
{
    std::string full {tmpl_dir};
    sss::path::append(full, command + ".tpl");
    switch (sss::path::file_exists(full)) {
    case sss::PATH_NOT_EXIST:
        sss::path::mkdir(full);
        // fall-throw:

    case sss::PATH_TO_DIRECTORY:
        std::cout << full;
        break;

    case sss::PATH_TO_FILE:
        // NOTE something wrong here!
        break;
    }
}

int main (int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    sss::PenvMgr2 env;
    env.set("tmpl_dir",
            sss::path::append_copy(sss::path::dirname(sss::path::getbin()),
                                   "template"));

    std::string get_command;
    std::string target;
    std::string dir = sss::path::getcwd();

    int idx = 1;

    if (idx < argc) {
        if (argv[idx] == std::string("--help")) {
            help_msg();
            return EXIT_SUCCESS;
        }
        else if (argv[idx] == std::string("--list")) {
            list_cmd(env.get("tmpl_dir"));
            return EXIT_SUCCESS;
        }
        else if (idx + 1 < argc && argv[idx] == std::string("--edit")) {
            edit_tpl(env.get("tmpl_dir"), argv[idx + 1]);
            return EXIT_SUCCESS;
        }
    }

    if (idx < argc) {
        get_command = argv[idx++];
        std::cout << NAME_WAPPER(get_command) << std::endl;
    }

    if (idx < argc) {
        target = argv[idx++];
        std::cout << NAME_WAPPER(target) << std::endl;
    }

    if (idx < argc) {
        dir = sss::path::full_of_copy(argv[idx++]);
        std::cout << NAME_WAPPER(dir) << std::endl;
    }

    if (get_command.empty()) {
        help_msg();
        return EXIT_SUCCESS;
    }
    // std::cout << NAME_WAPPER(dir) << std::endl;

    if (target.empty()) {
        target = '.';
        std::cout << "using current working dir" << std::endl;
    }
    get_command = sss::path::append_copy(env.get("tmpl_dir"), get_command + ".tpl");
    if (sss::path::file_exists(get_command) != sss::PATH_TO_DIRECTORY) {
        std::cerr << get_command << " not exists!" << std::endl;
        return EXIT_FAILURE;
    }

    env.set("Target", target);
    env.set("timestamp", sss::time::strftime("%Y%m%d"));

    std::cout << NAME_WAPPER(get_command) << std::endl;

    sss::path::file_descriptor fd;
    sss::path::glob_path_recursive gp(get_command, fd);
    while (gp.fetch()) {
        if (fd.is_normal_dir()) {
            std::string out_path = sss::path::append_copy(dir,
                                                          env.get_expr(sss::path::relative_to(fd.get_path(), get_command)));
            sss::path::mkpath(out_path);
            std::cout << sss::path::relative_to(out_path, dir) << "/" << " <- `mkdir -p`" << std::endl;
        }
        else if (fd.is_normal_file()) {
            std::string out_path = sss::path::append_copy(dir,
                                                          env.get_expr(sss::path::relative_to(fd.get_path(), get_command)));

            std::string content;
            bool using_template = false;
            if (sss::path::suffix(out_path) == ".tpl") {
                using_template = true;
                out_path = sss::path::no_suffix(out_path);
            }

            if (sss::path::file_exists(out_path)) {
                std::cerr << out_path << " already exists!" << std::endl;
                continue;
            }

            std::cout << sss::path::relative_to(out_path, dir) << " <- " << (using_template ? "template" : "copy") << std::endl;

            sss::path::file2string(fd.get_path(), content);
            std::ofstream ofs(out_path.c_str(), std::ios_base::out | std::ios_base::binary);
            if (using_template) {
                ofs << env.get_expr(content);
            }
            else {
                ofs << content;
            }
            sss::path::chmod(out_path, sss::path::getmod(fd.get_path()));
        }
    }

    return EXIT_SUCCESS;
}
