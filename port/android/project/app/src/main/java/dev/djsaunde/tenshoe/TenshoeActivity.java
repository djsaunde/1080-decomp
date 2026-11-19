/* port/android (plan 31): the app's activity. SDLActivity (SDL2's Java side) does the rest: the surface, input,
 * audio, and calling SDL_main in libmain.so (port/android/android_main.c) on its SDL thread.
 *
 * requestRom(message) is called from SDL_main (JNI) when no ROM is present: a dialog, then the system file picker
 * (ACTION_OPEN_DOCUMENT); the chosen file is copied into the app's internal files directory as baserom.z64 (through
 * baserom.z64.part and a rename, so SDL_main never sees half a file). SDL_main checks it is a 1080 Snowboarding ROM
 * and asks again if not. */
package dev.djsaunde.tenshoe;

import android.app.AlertDialog;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import org.libsdl.app.SDLActivity;

public class TenshoeActivity extends SDLActivity {
    private static final int PICK_ROM = 1080;
    private static final long MAX_ROM = 64L << 20;

    @Override
    protected String[] getLibraries() {
        return new String[] { "SDL2", "main" };
    }

    /* JNI, from the SDL thread */
    public void requestRom(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                showRomDialog(message);
            }
        });
    }

    private void showRomDialog(String message) {
        if (isFinishing()) {
            return;
        }
        new AlertDialog.Builder(this)
            .setTitle("1080 Snowboarding")
            .setMessage(message + "\n\nOr copy it to Android/data/" + getPackageName()
                        + "/files/baserom.z64 and start the app again.")
            .setCancelable(false)
            .setPositiveButton("Choose ROM", (d, w) -> openPicker())
            .setNegativeButton("Quit", (d, w) -> finish())
            .show();
    }

    private void openPicker() {
        Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        i.addCategory(Intent.CATEGORY_OPENABLE);
        i.setType("*/*");
        try {
            startActivityForResult(i, PICK_ROM);
        } catch (Exception e) {
            showRomDialog("No file picker is available (" + e.getMessage() + ").");
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode != PICK_ROM) {
            super.onActivityResult(requestCode, resultCode, data);
            return;
        }
        if (resultCode != RESULT_OK || data == null || data.getData() == null) {
            showRomDialog("No file was chosen. The game needs your 1080 Snowboarding ROM.");
            return;
        }
        final Uri uri = data.getData();
        new Thread(new Runnable() {
            @Override
            public void run() {
                final String err = copyRom(uri);
                if (err != null) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            showRomDialog("Could not copy that file: " + err);
                        }
                    });
                }
            }
        }, "RomCopy").start();
    }

    /* null on success */
    private String copyRom(Uri uri) {
        File dir = getFilesDir();
        File part = new File(dir, "baserom.z64.part");
        File rom = new File(dir, "baserom.z64");
        long total = 0;
        try (InputStream in = getContentResolver().openInputStream(uri);
             OutputStream out = new FileOutputStream(part)) {
            if (in == null) {
                return "the picker gave no data";
            }
            byte[] buf = new byte[1 << 16];
            int n;
            while ((n = in.read(buf)) > 0) {
                total += n;
                if (total > MAX_ROM) {
                    part.delete();
                    return "it is larger than an N64 ROM";
                }
                out.write(buf, 0, n);
            }
        } catch (Exception e) {
            part.delete();
            return e.getMessage();
        }
        if (total == 0) {
            part.delete();
            return "it is empty";
        }
        if (!part.renameTo(rom)) {
            part.delete();
            return "cannot write " + rom.getPath();
        }
        return null;
    }
}
