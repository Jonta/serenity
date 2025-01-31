/*
 * Copyright (c) 2021, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2021, Luke Wilde <lukew@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/NonnullRefPtrVector.h>
#include <LibJS/Forward.h>
#include <LibJS/Runtime/JobCallback.h>
#include <LibJS/Runtime/VM.h>
#include <LibWeb/DOM/MutationObserver.h>
#include <LibWeb/HTML/EventLoop/EventLoop.h>

namespace Web::Bindings {

struct WebEngineCustomData final : public JS::VM::CustomData {
    virtual ~WebEngineCustomData() override = default;

    HTML::EventLoop event_loop;

    // FIXME: These should only be on similar-origin window agents, but we don't currently differentiate agent types.

    // https://dom.spec.whatwg.org/#mutation-observer-compound-microtask-queued-flag
    bool mutation_observer_microtask_queued { false };

    // https://dom.spec.whatwg.org/#mutation-observer-list
    // FIXME: This should be a set.
    NonnullRefPtrVector<DOM::MutationObserver> mutation_observers;
};

struct WebEngineCustomJobCallbackData final : public JS::JobCallback::CustomData {
    WebEngineCustomJobCallbackData(HTML::EnvironmentSettingsObject& incumbent_settings, OwnPtr<JS::ExecutionContext> active_script_context)
        : incumbent_settings(incumbent_settings)
        , active_script_context(move(active_script_context))
    {
    }

    virtual ~WebEngineCustomJobCallbackData() override = default;

    HTML::EnvironmentSettingsObject& incumbent_settings;
    OwnPtr<JS::ExecutionContext> active_script_context;
};

HTML::ClassicScript* active_script();
JS::VM& main_thread_vm();
void queue_mutation_observer_microtask(DOM::Document&);

}
