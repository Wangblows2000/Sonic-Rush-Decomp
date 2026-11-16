// Export Ghidra decompiler output for every function into one file per
// function: <out-dir>/<name>.c. Drafts for matching work ONLY: never commit
// (derived from the ROM) and never treat as original source.
// Headless args: <out-dir> [timeoutSeconds]
//@category SonicRush
import java.io.*;
import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;

public class ExportDecomp extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File out = new File(args.length > 0 ? args[0] : "ghidra_decomp");
        int timeout = args.length > 1 ? Integer.parseInt(args[1]) : 30;
        out.mkdirs();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        int ok = 0, bad = 0;
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
            if (monitor.isCancelled()) break;
            DecompileResults r = di.decompileFunction(f, timeout, monitor);
            if (r != null && r.decompileCompleted()) {
                try (PrintWriter w = new PrintWriter(new File(out, f.getName() + ".c"))) {
                    w.printf("// Ghidra draft for %s @ 0x%08x (NOT original source; do not commit)%n",
                             f.getName(), f.getEntryPoint().getOffset());
                    w.print(r.getDecompiledFunction().getC());
                }
                ok++;
            } else {
                bad++;
            }
        }
        di.dispose();
        println("ExportDecomp: " + ok + " functions written, " + bad + " failed -> " + out);
    }
}
