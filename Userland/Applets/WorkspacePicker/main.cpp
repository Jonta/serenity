/*
 * Copyright (c) 2021, Peter Elliott <pelliott@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "DesktopStatusWindow.h"
#include <LibCore/System.h>
#include <LibGUI/Application.h>
#include <LibGUI/ConnectionToWindowManagerServer.h>
#include <LibGUI/Desktop.h>
#include <LibGUI/Painter.h>
#include <LibMain/Main.h>
#include <WindowServer/Window.h>

ErrorOr<int> serenity_main(Main::Arguments arguments)
{
    TRY(Core::System::pledge("stdio recvfd sendfd rpath unix"));

    auto app = TRY(GUI::Application::try_create(arguments));

    // We need to obtain the WM connection here as well before the pledge shortening.
    GUI::ConnectionToWindowManagerServer::the();

    TRY(Core::System::pledge("stdio recvfd sendfd rpath"));

    auto window = TRY(DesktopStatusWindow::try_create());
    window->set_title("WorkspacePicker");

    auto& desktop = GUI::Desktop::the();
    if (desktop.workspace_rows() == 1 && desktop.workspace_columns() == 1)
        window->resize(0, 0);
    else
        window->resize(28, 16);

    window->show();
    window->make_window_manager(WindowServer::WMEventMask::WorkspaceChanges);

    return app->exec();
}
