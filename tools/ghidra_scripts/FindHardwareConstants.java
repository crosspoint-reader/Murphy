// Headless Ghidra script for mining ESP32 firmware for hardware-looking
// constants and functions. It reports functions/symbols matching keywords,
// instructions whose operands mention selected constants, and decompilation
// snippets for matching functions.

import java.io.File;
import java.io.PrintWriter;
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;

public class FindHardwareConstants extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 2) {
			printerr("Usage: FindHardwareConstants <out.md> <keyword-or-number> [keyword-or-number...]");
			return;
		}

		DecompInterface ifc = new DecompInterface();
		ifc.setOptions(new DecompileOptions());
		if (!ifc.openProgram(currentProgram)) {
			throw new RuntimeException("Decompiler open failed");
		}

		try (PrintWriter pw = new PrintWriter(new File(args[0]))) {
			pw.println("# Hardware Constant / Function Mining");
			pw.println();
			reportMatchingFunctions(pw, ifc, args);
			reportMatchingSymbols(pw, args);
			reportMatchingInstructions(pw, ifc, args);
		} finally {
			ifc.dispose();
		}
		printf("Wrote %s%n", new File(args[0]).getAbsolutePath());
	}

	private void reportMatchingFunctions(PrintWriter pw, DecompInterface ifc, String[] args) throws Exception {
		pw.println("## Matching Function Names");
		pw.println();
		Set<String> dumped = new HashSet<>();
		FunctionIterator funcs = currentProgram.getFunctionManager().getFunctions(true);
		for (Function fn : funcs) {
			String name = fn.getName(true);
			String lower = name.toLowerCase(Locale.ROOT);
			for (int i = 1; i < args.length; i++) {
				if (parseNumber(args[i]) != null) {
					continue;
				}
				if (lower.contains(args[i].toLowerCase(Locale.ROOT))) {
					pw.printf("- `%s` at `%s`%n", name, fn.getEntryPoint());
					if (dumped.add(fn.getEntryPoint().toString())) {
						dumpDecompile(pw, ifc, fn, 120);
					}
					break;
				}
			}
		}
		pw.println();
	}

	private void reportMatchingSymbols(PrintWriter pw, String[] args) {
		pw.println("## Matching Symbols");
		pw.println();
		SymbolIterator symbols = currentProgram.getSymbolTable().getSymbolIterator();
		while (symbols.hasNext() && !monitor.isCancelled()) {
			Symbol sym = symbols.next();
			String name = sym.getName(true);
			String lower = name.toLowerCase(Locale.ROOT);
			for (int i = 1; i < args.length; i++) {
				if (parseNumber(args[i]) != null) {
					continue;
				}
				if (lower.contains(args[i].toLowerCase(Locale.ROOT))) {
					pw.printf("- `%s` `%s` at `%s`%n", sym.getSymbolType(), name, sym.getAddress());
					break;
				}
			}
		}
		pw.println();
	}

	private void reportMatchingInstructions(PrintWriter pw, DecompInterface ifc, String[] args) throws Exception {
		pw.println("## Matching Instructions");
		pw.println();
		Set<String> dumped = new HashSet<>();
		Instruction ins = getFirstInstruction();
		while (ins != null && !monitor.isCancelled()) {
			String text = instructionText(ins).toLowerCase(Locale.ROOT);
			for (int i = 1; i < args.length; i++) {
				String arg = args[i].toLowerCase(Locale.ROOT);
				Long number = parseNumber(arg);
				String hex = number == null ? null : "0x" + Long.toHexString(number);
				if (number != null && !text.contains(hex)) {
					continue;
				}
				if (text.contains(arg) || (hex != null && text.contains(hex))) {
					Function fn = getFunctionContaining(ins.getAddress());
					pw.printf("- `%s` in `%s`%n", instructionText(ins),
						fn == null ? "<no function>" : fn.getName(true));
					if (fn != null && dumped.add(fn.getEntryPoint().toString())) {
						dumpNearby(pw, ins.getAddress(), 18);
						dumpDecompile(pw, ifc, fn, 180);
					}
					break;
				}
			}
			ins = getInstructionAfter(ins.getAddress());
		}
	}

	private String parseHexNeedle(String arg) {
		Long v = parseNumber(arg);
		return v == null ? null : "0x" + Long.toHexString(v);
	}

	private Long parseNumber(String arg) {
		try {
			if (arg.startsWith("0x")) {
				return Long.parseUnsignedLong(arg.substring(2), 16);
			}
			if (arg.matches("[0-9]+")) {
				return Long.parseLong(arg, 10);
			}
			return null;
		} catch (Exception e) {
			return null;
		}
	}

	private String instructionText(Instruction ins) {
		StringBuilder sb = new StringBuilder();
		sb.append(ins.getAddress()).append("  ").append(ins.getMnemonicString());
		for (int op = 0; op < ins.getNumOperands(); op++) {
			sb.append(op == 0 ? " " : ", ");
			sb.append(ins.getDefaultOperandRepresentation(op));
		}
		return sb.toString();
	}

	private void dumpNearby(PrintWriter pw, Address center, int count) {
		pw.println("  nearby:");
		pw.println("  ```asm");
		Instruction ins = getInstructionAt(center);
		for (int i = 0; i < count / 2 && ins != null; i++) {
			Instruction prev = getInstructionBefore(ins.getAddress());
			if (prev == null) break;
			ins = prev;
		}
		for (int i = 0; i < count && ins != null; i++) {
			pw.print("  ");
			pw.println(instructionText(ins));
			ins = getInstructionAfter(ins.getAddress());
		}
		pw.println("  ```");
	}

	private void dumpDecompile(PrintWriter pw, DecompInterface ifc, Function fn, int maxLines) throws Exception {
		pw.printf("  decompile `%s` at `%s`:%n", fn.getName(true), fn.getEntryPoint());
		DecompileResults res = ifc.decompileFunction(fn, 90, monitor);
		if (!res.decompileCompleted()) {
			pw.printf("  decompiler failed: %s%n", res.getErrorMessage());
			return;
		}
		pw.println("  ```c");
		String[] lines = res.getDecompiledFunction().getC().split("\\R");
		for (int i = 0; i < lines.length && i < maxLines; i++) {
			pw.print("  ");
			pw.println(lines[i]);
		}
		if (lines.length > maxLines) {
			pw.println("  /* truncated */");
		}
		pw.println("  ```");
	}
}
