// Copyright (c) 2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MALLIOR_NODE_CHAINSTATEMANAGER_ARGS_H
#define MALLIOR_NODE_CHAINSTATEMANAGER_ARGS_H

#include <validation.h>

#include <optional>

class ArgsManager;
struct bilingual_str;

namespace node {
std::optional<bilingual_str> ApplyArgsManOptions(const ArgsManager& args, ChainstateManager::Options& opts);
} // namespace node

#endif // MALLIOR_NODE_CHAINSTATEMANAGER_ARGS_H
