// Copyright (c) 2011-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MALLIOR_QT_MALLIORADDRESSVALIDATOR_H
#define MALLIOR_QT_MALLIORADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class MalliorAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit MalliorAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Mallior address widget validator, checks for a valid mallior address.
 */
class MalliorAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit MalliorAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // MALLIOR_QT_MALLIORADDRESSVALIDATOR_H
