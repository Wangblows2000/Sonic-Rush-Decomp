// Apply a dsd symbols.txt to the current program and optionally add extra
// memory blocks (other modules) for cross-reference context.
//
// Headless args:  <symbols.txt> [<block-name>=<file>@<hexaddr> ...]
//
// - Thumb functions get TMode=1 over their extent before disassembly.
// - Default dsd names (func_XXXXXXXX, data_...) are applied with
//   SourceType.ANALYSIS; any other name with SourceType.IMPORTED so that the
//   distinction between unknown and named symbols survives in Ghidra.
//
// STATUS: used with Ghidra 12.2 + JDK 25 via tools/ghidra/import.sh; Thumb
// context is set for all functions before disassembly (docs/research/0007-ghidra.md).
//@category SonicRush

import java.io.*;
import java.math.BigInteger;
import java.nio.file.*;
import java.util.regex.*;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.SourceType;

public class ApplyDsdSymbols extends GhidraScript {
    private static final Pattern SYM = Pattern.compile(
        "^(\\S+) kind:(\\w+)(?:\\(([^)]*)\\))? addr:0x([0-9a-fA-F]+)");
    private static final Pattern DEFAULT_NAME = Pattern.compile(
        "^(func|data|bss|__sinit)_(ov\\d{3}_)?[0-9a-f]{8}$");

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            printerr("usage: ApplyDsdSymbols <symbols.txt> [name=file@addr ...]");
            return;
        }
        for (int i = 1; i < args.length; i++) addBlock(args[i]);

        Register tmode = currentProgram.getRegister("TMode");
        int funcs = 0, labels = 0, failed = 0;
        java.util.List<String> lines = Files.readAllLines(Paths.get(args[0]));
        // Pass 1: set Thumb/ARM context over every function range before any
        // disassembly, so auto-flow from earlier functions cannot lock in ARM.
        if (tmode != null) {
            for (String line : lines) {
                Matcher m = SYM.matcher(line);
                if (!m.find() || !m.group(2).equals("function")) continue;
                String opts = m.group(3) == null ? "" : m.group(3);
                Address a = toAddr(Long.parseLong(m.group(4), 16));
                if (currentProgram.getMemory().getBlock(a) == null) continue;
                long size = 2;
                Matcher sm = Pattern.compile("size=0x([0-9a-fA-F]+)").matcher(opts);
                if (sm.find()) size = Long.parseLong(sm.group(1), 16);
                try {
                    currentProgram.getProgramContext().setValue(tmode, a, a.add(Math.max(size, 2) - 1),
                        opts.contains("thumb") ? BigInteger.ONE : BigInteger.ZERO);
                } catch (Exception e) { /* reported in pass 2 */ }
            }
        }
        for (String line : lines) {
            Matcher m = SYM.matcher(line);
            if (!m.find()) continue;
            String name = m.group(1), kind = m.group(2);
            String opts = m.group(3) == null ? "" : m.group(3);
            long addr = Long.parseLong(m.group(4), 16);
            if (name.startsWith(".L_")) continue;
            Address a = toAddr(addr);
            if (currentProgram.getMemory().getBlock(a) == null) continue;
            SourceType src = DEFAULT_NAME.matcher(name).matches()
                ? SourceType.ANALYSIS : SourceType.IMPORTED;
            try {
                if (kind.equals("function")) {
                    boolean thumb = opts.contains("thumb");
                    long size = 2;
                    Matcher sm = Pattern.compile("size=0x([0-9a-fA-F]+)").matcher(opts);
                    if (sm.find()) size = Long.parseLong(sm.group(1), 16);
                    disassemble(a);
                    Function f = getFunctionAt(a);
                    if (f == null) f = createFunction(a, name);
                    if (f != null) { f.setName(name, src); funcs++; } else failed++;
                } else {
                    createLabel(a, name, true, src);
                    labels++;
                }
            } catch (Exception e) {
                failed++;
                println("warn: " + name + " @" + a + ": " + e.getMessage());
            }
        }
        println(String.format("ApplyDsdSymbols: %d functions, %d labels, %d failed",
                              funcs, labels, failed));
    }

    private void addBlock(String spec) throws Exception {
        // name=path@hexaddr
        String[] kv = spec.split("=", 2);
        String[] fa = kv[1].split("@", 2);
        File f = new File(fa[0]);
        Address start = toAddr(Long.parseLong(fa[1].replace("0x", ""), 16));
        Memory mem = currentProgram.getMemory();
        try (InputStream in = new FileInputStream(f)) {
            MemoryBlock b = mem.createInitializedBlock(kv[0], start, in, f.length(),
                                                       monitor, false);
            b.setExecute(true);
            b.setRead(true);
            b.setWrite(true);
        }
        println("added block " + kv[0] + " @" + start + " size " + f.length());
    }
}
