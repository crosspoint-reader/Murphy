// Decompiles functions containing supplied addresses.

import java.io.File;
import java.io.PrintWriter;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class DecompileAt extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 2) {
			printerr("Usage: DecompileAt <out.md> <address> [address...]");
			return;
		}
		DecompInterface ifc = new DecompInterface();
		DecompileOptions opts = new DecompileOptions();
		ifc.setOptions(opts);
		if (!ifc.openProgram(currentProgram)) {
			throw new RuntimeException("Decompiler open failed");
		}
		try (PrintWriter pw = new PrintWriter(new File(args[0]))) {
			pw.println("# Decompile Report");
			pw.println();
			for (int i = 1; i < args.length; i++) {
				Address addr = toAddr(Long.decode(args[i]));
				Function fn = getFunctionContaining(addr);
				if (fn == null) {
					pw.printf("## `%s`%n%nNo function containing address.%n%n", args[i]);
					continue;
				}
				pw.printf("## `%s` in `%s` at `%s`%n%n", args[i], fn.getName(true), fn.getEntryPoint());
				DecompileResults res = ifc.decompileFunction(fn, 90, monitor);
				if (!res.decompileCompleted()) {
					pw.printf("Decompiler failed: %s%n%n", res.getErrorMessage());
					continue;
				}
				pw.println("```c");
				pw.println(res.getDecompiledFunction().getC());
				pw.println("```");
				pw.println();
			}
		}
		ifc.dispose();
	}
}
