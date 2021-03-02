#ifndef CLI_COMMON_H
#define CLI_COMMON_H

#include <iostream>

#include <QStringList>
#include <QCommandLineParser>

enum ErrCode
{
    UnrecognizedSwitch=1,
    UnrecognizedOption,
    UnrecognizedCommand,
    InvalidSwitch,
    InvalidOption,
    InvalidCommand,
    InvalidValue,
    MissingPosArg,
    FileError,
};

void unknown_option(const QString &opt, const QString &cmd = "schim");
void unknown_command(const QString &cmd);
void missing_positional(const QString &arg, const QString &cmd = "schim");
void too_many_arguments(const QString &cmd = "schim");

#endif // CLI_COMMON_H
