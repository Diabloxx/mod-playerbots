# CMake entries for Ruby Sanctum raid strategy
# This file allows building the new raid strategy sources when module builds

set(RUBY_SANCTUM_SRCS
    RaidRubySanctumStrategy.cpp
    RaidRubySanctumActionContext.cpp
    RaidRubySanctumTriggers.h
    RaidRubySanctumTriggerContext.h
)

core_add_library(mod_playerbots_rubysanctum SRCS ${RUBY_SANCTUM_SRCS})
