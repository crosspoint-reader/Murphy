// Finds instructions whose rendered operands contain supplied addresses and
// reports their containing functions. Useful for Xtensa L32R literal pools.

import java.io.File;
import java.io.PrintWriter;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;

public class FindOperandUses extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 2) {
			printerr("Usage: FindOperandUses <out.md> <operand-text> [operand-text...]");
			return;
		}
		try (PrintWriter pw = new PrintWriter(new File(args[0]))) {
			pw.println("# Instruction Operand Uses\n");
			for (int n = 1; n < args.length; n++) {
				String needle = args[n].toLowerCase();
				pw.printf("## `%s`%n%n", args[n]);
				Instruction ins = getFirstInstruction();
				while (ins != null && !monitor.isCancelled()) {
					StringBuilder rendered = new StringBuilder();
					for (int op = 0; op < ins.getNumOperands(); op++) {
						if (op > 0) rendered.append(", ");
						rendered.append(ins.getDefaultOperandRepresentation(op));
					}
					if (rendered.toString().toLowerCase().contains(needle)) {
						Function fn = getFunctionContaining(ins.getAddress());
						pw.printf("- `%s`: `%s %s`, function `%s` at `%s`%n", ins.getAddress(),
							ins.getMnemonicString(), rendered,
							fn == null ? "<none>" : fn.getName(),
							fn == null ? "<none>" : fn.getEntryPoint());
					}
					ins = getInstructionAfter(ins.getAddress());
				}
				pw.println();
			}
		}
	}
}
