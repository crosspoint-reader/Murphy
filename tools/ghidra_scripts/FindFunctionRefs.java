// Reports references to functions containing supplied addresses and decompiles callers.

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;

import java.io.File;
import java.io.PrintWriter;
import java.util.HashSet;
import java.util.Set;

public class FindFunctionRefs extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 2) {
			printerr("Usage: FindFunctionRefs <out.md> <address> [address...]");
			return;
		}

		DecompInterface ifc = new DecompInterface();
		ifc.setOptions(new DecompileOptions());
		if (!ifc.openProgram(currentProgram)) {
			throw new RuntimeException("Decompiler open failed");
		}

		try (PrintWriter pw = new PrintWriter(new File(args[0]))) {
			pw.println("# Function Reference Report");
			Set<String> dumped = new HashSet<>();
			for (int i = 1; i < args.length; i++) {
				Address addr = toAddr(Long.decode(args[i]));
				Function target = getFunctionContaining(addr);
				if (target == null) {
					pw.printf("%n## `%s`%n%nNo containing function.%n", addr);
					continue;
				}

				pw.printf("%n## Target `%s` at `%s`%n%n", target.getName(), target.getEntryPoint());
				Reference[] refs = getReferencesTo(target.getEntryPoint());
				pw.printf("Reference count: %d%n%n", refs.length);
				for (Reference ref : refs) {
					Address from = ref.getFromAddress();
					Function caller = getFunctionContaining(from);
					pw.printf("- `%s` from `%s` in `%s`%n", ref.getReferenceType(), from,
							caller == null ? "<none>" : caller.getName() + " @ " + caller.getEntryPoint());
					dumpNearby(pw, from, 10);
					if (caller != null) {
						String key = caller.getEntryPoint().toString();
						if (!dumped.contains(key)) {
							dumped.add(key);
							dumpDecompile(pw, ifc, caller);
						}
					}
				}
			}
		} finally {
			ifc.dispose();
		}
	}

	private void dumpNearby(PrintWriter pw, Address center, int count) {
		pw.println();
		pw.println("```asm");
		ghidra.program.model.listing.Instruction ins = getInstructionAt(center);
		if (ins == null) ins = getInstructionBefore(center);
		for (int i = 0; i < count && ins != null; i++) {
			ghidra.program.model.listing.Instruction prev = getInstructionBefore(ins.getAddress());
			if (prev == null) break;
			ins = prev;
		}
		for (int i = 0; i < count * 2 && ins != null; i++) {
			StringBuilder ops = new StringBuilder();
			for (int op = 0; op < ins.getNumOperands(); op++) {
				if (op > 0) ops.append(", ");
				ops.append(ins.getDefaultOperandRepresentation(op));
			}
			pw.printf("%s  %-12s %s%n", ins.getAddress(), ins.getMnemonicString(), ops.toString());
			ins = getInstructionAfter(ins.getAddress());
		}
		pw.println("```");
		pw.println();
	}

	private void dumpDecompile(PrintWriter pw, DecompInterface ifc, Function fn) throws Exception {
		pw.printf("### Caller decompile `%s` `%s`%n%n", fn.getName(), fn.getEntryPoint());
		DecompileResults res = ifc.decompileFunction(fn, 90, monitor);
		if (!res.decompileCompleted()) {
			pw.printf("Decompiler failed: %s%n%n", res.getErrorMessage());
			return;
		}
		pw.println("```c");
		pw.println(res.getDecompiledFunction().getC());
		pw.println("```");
		pw.println();
	}
}
