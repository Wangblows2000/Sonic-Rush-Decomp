// Export Ghidra's view of the program for comparison with dsd's analysis.
//
// Headless args: <out-dir>
// Writes:
//   functions.csv  entry,name,source,size,thumb,callers,callees
//   xrefs.csv      from,to,type
//   strings.csv    address,length,value
//
// tools/analysis/compare_ghidra.py diffs functions.csv against dsd symbols.
// STATUS: not yet executed in this project (see docs/research/0007-ghidra.md).
//@category SonicRush

import java.io.*;
import java.math.BigInteger;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.data.StringDataInstance;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;

public class ExportAnalysis extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File out = new File(args.length > 0 ? args[0] : "ghidra_export");
        out.mkdirs();
        Register tmode = currentProgram.getRegister("TMode");

        try (PrintWriter w = new PrintWriter(new File(out, "functions.csv"))) {
            w.println("entry,name,source,size,thumb,callers,callees");
            for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
                Address e = f.getEntryPoint();
                BigInteger t = tmode == null ? null
                    : currentProgram.getProgramContext().getValue(tmode, e, false);
                w.printf("0x%08x,%s,%s,%d,%d,%d,%d%n", e.getOffset(), f.getName(),
                    f.getSymbol().getSource(), f.getBody().getNumAddresses(),
                    (t != null && t.intValue() == 1) ? 1 : 0,
                    f.getCallingFunctions(monitor).size(),
                    f.getCalledFunctions(monitor).size());
            }
        }
        try (PrintWriter w = new PrintWriter(new File(out, "xrefs.csv"))) {
            w.println("from,to,type");
            ReferenceManager rm = currentProgram.getReferenceManager();
            for (Address src : rm.getReferenceSourceIterator(
                    currentProgram.getMemory(), true)) {
                for (Reference r : rm.getReferencesFrom(src)) {
                    if (!r.getToAddress().isMemoryAddress()) continue;
                    w.printf("0x%08x,0x%08x,%s%n", src.getOffset(),
                             r.getToAddress().getOffset(), r.getReferenceType());
                }
            }
        }
        try (PrintWriter w = new PrintWriter(new File(out, "strings.csv"))) {
            w.println("address,length,value");
            for (Data d : currentProgram.getListing().getDefinedData(true)) {
                if (!StringDataInstance.isString(d)) continue;
                StringDataInstance s = StringDataInstance.getStringDataInstance(d);
                String v = s.getStringValue();
                if (v == null) continue;
                w.printf("0x%08x,%d,\"%s\"%n", d.getAddress().getOffset(), d.getLength(),
                         v.replace("\"", "\"\"").replace("\n", "\\n"));
            }
        }
        println("ExportAnalysis: wrote " + out);
    }
}
