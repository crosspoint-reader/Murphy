// Reports references to named functions/strings and dumps the containing
// function decompile plus nearby instructions. Intended for ESP32 firmware
// hardware-callsite mining.

import java.io.File;
import java.io.PrintWriter;
import java.util.HashSet;
import java.util.Set;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.app.script.GhidraScript;

public class FindCallsiteContexts extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 2) {
			printerr("Usage: FindCallsiteContexts <out.md> <name-or-string> [name-or-string...]");
			return;
		}

		DecompInterface ifc = new DecompInterface();
		DecompileOptions opts = new DecompileOptions();
		ifc.setOptions(opts);
		if (!ifc.openProgram(currentProgram)) {
			throw new RuntimeException("Decompiler open failed");
		}

		try (PrintWriter pw = new PrintWriter(new File(args[0]))) {
			pw.println("# Hardware Callsite Contexts");
			pw.println();
			for (int i = 1; i < args.length; i++) {
				reportNeedle(pw, ifc, args[i]);
			}
		} finally {
			ifc.dispose();
		}
		printf("Wrote %s%n", new File(args[0]).getAbsolutePath());
	}

	private void reportNeedle(PrintWriter pw, DecompInterface ifc, String needle) throws Exception {
		pw.printf("## `%s`%n%n", needle);
		Set<String> seenFunctions = new HashSet<>();
		int refCount = 0;

		SymbolIterator symbols = currentProgram.getSymbolTable().getSymbols(needle);
		for (Symbol sym : symbols) {
			refCount += reportRefsToAddress(pw, ifc, needle, sym.getAddress(), seenFunctions);
		}

		Data data = getFirstData();
		while (data != null && !monitor.isCancelled()) {
			if (data.hasStringValue()) {
				String value = data.getDefaultValueRepresentation();
				if (value != null && value.contains(needle)) {
					pw.printf("- string `%s`: `%s`%n", data.getAddress(), value.replace("\n", "\\n"));
					refCount += reportRefsToAddress(pw, ifc, needle, data.getAddress(), seenFunctions);
				}
			}
			data = getDataAfter(data);
		}

		pw.printf("%nRefs reported: %d%n%n", refCount);
	}

	private int reportRefsToAddress(PrintWriter pw, DecompInterface ifc, String needle, Address target,
			Set<String> seenFunctions) throws Exception {
		int count = 0;
		Reference[] refs = getReferencesTo(target);
		for (Reference ref : refs) {
			count++;
			Address from = ref.getFromAddress();
			Function fn = getFunctionContaining(from);
			pw.printf("- ref to `%s` target `%s` from `%s` in `%s`%n", needle, target, from,
				fn == null ? "<no function>" : fn.getName(true));
			dumpNearbyInstructions(pw, from, 10);
			if (fn != null) {
				String key = fn.getEntryPoint().toString();
				if (!seenFunctions.contains(key)) {
					seenFunctions.add(key);
					dumpDecompile(pw, ifc, fn);
				}
			}
		}
		return count;
	}

	private void dumpNearbyInstructions(PrintWriter pw, Address center, int count) {
		pw.println("  nearby:");
		pw.println("  ```asm");
		Instruction ins = getInstructionAt(center);
		if (ins == null) {
			ins = getInstructionBefore(center);
		}
		for (int i = 0; i < count / 2 && ins != null; i++) {
			Instruction prev = getInstructionBefore(ins.getAddress());
			if (prev == null) {
				break;
			}
			ins = prev;
		}
		for (int i = 0; i < count && ins != null; i++) {
			StringBuilder operands = new StringBuilder();
			for (int op = 0; op < ins.getNumOperands(); op++) {
				if (op > 0) {
					operands.append(", ");
				}
				operands.append(ins.getDefaultOperandRepresentation(op));
			}
			pw.printf("  %s  %-12s %s%n", ins.getAddress(), ins.getMnemonicString(), operands.toString());
			ins = getInstructionAfter(ins.getAddress());
		}
		pw.println("  ```");
	}

	private void dumpDecompile(PrintWriter pw, DecompInterface ifc, Function fn) throws Exception {
		pw.printf("  decompile `%s` at `%s`:%n", fn.getName(true), fn.getEntryPoint());
		DecompileResults res = ifc.decompileFunction(fn, 90, monitor);
		if (!res.decompileCompleted()) {
			pw.printf("  decompiler failed: %s%n", res.getErrorMessage());
			return;
		}
		pw.println("  ```c");
		String[] lines = res.getDecompiledFunction().getC().split("\\R");
		for (int i = 0; i < lines.length && i < 220; i++) {
			pw.print("  ");
			pw.println(lines[i]);
		}
		if (lines.length > 220) {
			pw.println("  /* truncated */");
		}
		pw.println("  ```");
	}
}
