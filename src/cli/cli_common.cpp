#include "cli_common.h"

void unknown_option(const QString &opt, const QString &cmd)
{
    std::cerr << cmd.toStdString() << ": Unknown option '" << opt.toStdString() << "'.\n";
}

void unknown_command(const QString &cmd)
{
    std::cerr << "schim: Unknown command '" << cmd.toStdString() << "'.\n";
}

void missing_positional(const QString &arg, const QString &cmd)
{
    std::cerr << cmd.toStdString() << ": Missing positional argument '" + arg.toStdString() + "'.\n";
}

void too_many_arguments(const QString &cmd)
{
    std::cerr << cmd.toStdString() << ": Too many arguments.\n";
}
