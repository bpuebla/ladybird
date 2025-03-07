/*
 * Copyright (c) 2023, Tim Flynn <trflynn89@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGC/Ptr.h>
#include <LibWeb/Forward.h>

namespace Web::HTML {

// When JS is run from outside the context of any user script, we currently do not have a running execution context.
// This results in a crash when we access VM::running_execution_context(). This is a spec issue. Until it is resolved,
// this is a workaround to temporarily push an execution context.
class [[nodiscard]] TemporaryExecutionContext {
public:
    enum class CallbacksEnabled {
        No,
        Yes,
    };

    explicit TemporaryExecutionContext(JS::Realm&, CallbacksEnabled = CallbacksEnabled::No);
    ~TemporaryExecutionContext();

private:
    GC::Ref<JS::Realm> m_realm;
    CallbacksEnabled m_callbacks_enabled { CallbacksEnabled::No };
};

}
