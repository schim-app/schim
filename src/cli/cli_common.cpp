#include "cli_common.h"

void unknown_option(const QString &opt)
{
    std::cerr << "schim: Unknown option '" << opt.toStdString() << "'\n";
}

void unknown_command(const QString &cmd)
{
    std::cerr << "schim: Unknown command '" << cmd.toStdString() << "'\n";
}

void missing_positional(const QString &arg)
{
    std::cerr << "schim: Missing positional argument '" + arg.toStdString() + "'.";
}
