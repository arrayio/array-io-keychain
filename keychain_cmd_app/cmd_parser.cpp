//
// Created by roman on 4/6/18.
//


#include <boost/config/detail/select_platform_config.hpp>
#include <iostream>

#include <keychain_lib/pipeline_parser.hpp>
#include <keychain_lib/keychain_wrapper.hpp>
#include <keychain_lib/secure_module_singletone.hpp>

#include "cmd_parser.hpp"
#include "sec_mod.hpp"

#include <boost/program_options.hpp>
//#include <boost/program_options/options_description.hpp>
//#include <boost/program_options/option.hpp>

#ifdef LINUX
    #include "sec_mod_linux.hpp"
#endif

using namespace keychain_app;


cmd_parser::cmd_parser()
{
  //TODO: define program options
}

int cmd_parser::run(int argc, const char* const argv [])
{

  const secure_dlg_mod_base* sec_mod;

  std::string task_type;
  po::options_description desc("Options");
  desc.add_options()
          ("help,h", "Show help")
          ("mode", po::value<std::string>(&task_type), "Select mode=test_run for test program with \"blank\" password");

  po::variables_map vm;
  try
  {
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsed, vm);
    po::notify(vm);

    if (task_type == "test_run")
      sec_mod = secure_module<sec_mod_dummy>::instance();
    else if (task_type == "")
    {
#ifdef LINUX
       sec_mod = secure_module<sec_mod_linux>::instance();
#else
       sec_mod = secure_module<sec_mod_dummy>::instance();
#endif
    }
    else
      std::cout<< desc << std::endl;
  }
  catch (std::exception& ex)
  {
    std::cout<< desc << std::endl;
    return 0;
  }

  keychain_invoke_f f = std::bind(&keychain_wrapper, sec_mod, std::placeholders::_1);
  pipeline_parser pipe_line_parser_(std::move(f), fileno(stdin), fileno(stdout));
  return pipe_line_parser_.run();
}
