/*******************************************************
 * Copyright (c) 2020, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#pragma once

#include <common/ModuleInterface.hpp>
#include <cu_check_macro.hpp>

#include <cuda.h>

#include <string>
#include <unordered_map>

namespace cuda {

/// CUDA backend wrapper for CUmodule
class Module : public common::ModuleInterface<CUmodule> {
   private:
    std::unordered_map<std::string, std::string> mInstanceMangledNames;

   public:
    using ModuleType = CUmodule;
    using BaseClass  = common::ModuleInterface<ModuleType>;

    Module(ModuleType mod) : BaseClass(mod) {
        mInstanceMangledNames.reserve(1);
    }

    void unload() final {
        CU_CHECK(cuModuleUnload(get()));
        set(nullptr);
    }

    const std::string mangledName(const std::string& instantiation) const {
        auto iter = mInstanceMangledNames.find(instantiation);
        if (iter != mInstanceMangledNames.end()) {
            return iter->second;
        } else {
            return std::string("");
        }
    }

    void add(const std::string& instantiation, const std::string& mangledName) {
        mInstanceMangledNames.emplace(instantiation, mangledName);
    }

    const auto& map() const { return mInstanceMangledNames; }
};

}  // namespace cuda
