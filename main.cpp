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

    std::cout << NAME_WAPPER(dir) << std::endl;

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
