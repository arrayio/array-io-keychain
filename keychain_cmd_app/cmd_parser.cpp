//
// Created by roman on 4/6/18.
//


#include <boost/config/detail/select_platform_config.hpp>
#include <iostream>

#include <keychain_lib/pipeline_parser.hpp>
#include <keychain_lib/keychain.hpp>
#include <keychain_lib/mod_singleton.hpp>

#include "cmd_parser.hpp"
#include "sec_mod.hpp"
#include "gui_mod.hpp"

#include <boost/program_options.hpp>
#include <keychain_lib/keychain_logger.hpp>

#include <keychain_lib/keydata_singleton.hpp>

//#include <boost/program_options/options_description.hpp>
//#include <boost/program_options/option.hpp>

#ifdef LINUX
#include "sec_mod_linux.hpp"
#endif

#ifdef APPLE
#include "../keychain_mac/sec_mod_mac.hpp"
#include "../keychain_mac/gui_mod_mac.hpp"
#endif

#ifdef _WIN32
#include <SecureModuleWrapper.h>
#endif

using namespace keychain_app;

cmd_parser::cmd_parser()
{
  //TODO: define program options
}

int cmd_parser::run(int argc, const char* const argv[])
{

  const secure_dlg_mod_base* sec_mod;
  const gui_mod_base* gui_mod;
  auto& log = logger_singleton::instance();

  std::string task_type;
  po::options_description desc("Options");
  desc.add_options()
    ("help,h", "Show help")
    ("mode", po::value<std::string>(&task_type), "Select mode=test_run for test program with \"blank\" password");

  po::variables_map options;
  try
  {
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();

    po::store(parsed, options);
    po::notify(options);

    if (options.count("help"))
    {
      std::cout << desc << std::endl;
      return 0;
    }

    if (task_type == "test_run")
    {
      BOOST_LOG_SEV(log.lg, info) << "secure_module: <sec_mod_dummy>";
      sec_mod = module_singleton<secure_dlg_mod_base>::instance<sec_mod_dummy>();
      gui_mod = module_singleton<gui_mod_base>::instance<gui_mod_dummy>();
    }
    else if (task_type == "")
    {
#ifdef LINUX
      BOOST_LOG_SEV(log.lg, info) << "secure_module: <sec_mod_linux>";
      sec_mod = module_singleton<secure_dlg_mod_base>::instance<sec_mod_linux>();
      //TODO: need to implement gui module
      gui_mod = module_singleton<gui_mod_base>::instance<gui_mod_dummy>();
#else
#	if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
      BOOST_LOG_SEV(log.lg, info) << "secure_module: <sec_mod_mac>";
      sec_mod = module_singleton<secure_dlg_mod_base>::instance<sec_mod_mac>();
      //TODO: need to implement gui module
      gui_mod = module_singleton<gui_mod_base>::instance<gui_mod_mac>();
#	else
#		ifdef _WIN32
      BOOST_LOG_SEV(log.lg, info) << "secure_module: <SecureModuleWrapper>";
      sec_mod = module_singleton<secure_dlg_mod_base>::instance<SecureModuleWrapper>();
      //TODO: need to implement gui module
      gui_mod = module_singleton<gui_mod_base>::instance<gui_mod_dummy>();
#		endif //_WIN32
#	endif //APPLE
#endif //LINUX
    }
    else
    {
      std::cout << desc << std::endl;
      return 0;
    }

  }
  catch (std::exception& ex)
  {
    std::cout << desc << std::endl;
    return 0;
  }

  auto& keychain_ref = keychain::instance();
  sec_mod->connect(keychain_ref);
  gui_mod->connect(keychain_ref);

  auto& keyfiles = keyfile_singleton::instance();
  auto it = keyfiles.begin();
  if ( it==keyfiles.end() )
  {
      auto res = keychain_ref.entropy();
      auto& key_data = keydata_singleton::instance();
      dev::bytes ue;
      auto mnemonics = std::move(key_data.seed(ue));
      std::string pass("blank");
      key_data.create_masterkey(mnemonics,pass);
//      key_data.create_privatekey();
  }

  keychain_invoke_f f = std::bind(&keychain_base::operator(), &keychain_ref, std::placeholders::_1);
  pipeline_parser pipe_line_parser_(std::move(f), fileno(stdin), fileno(stdout));
  return pipe_line_parser_.run();
}
