package org.malliorcore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class MalliorQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File malliorDir = new File(getFilesDir().getAbsolutePath() + "/.mallior");
        if (!malliorDir.exists()) {
            malliorDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
