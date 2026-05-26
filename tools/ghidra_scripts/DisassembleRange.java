// Force-disassembles ranges and dumps instructions.

import java.io.File;
import java.io.PrintWriter;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;

public class DisassembleRange extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 4 || ((args.length - 1) % 2) != 0) {
			printerr("Usage: DisassembleRange <out.md> <start> <length> [<start> <length>...]");
			return;
		}
		try (PrintWriter pw = new PrintWriter(new File(args[0]))) {
			pw.println("# Disassembly Report");
			pw.println();
			for (int i = 1; i < args.length; i += 2) {
				Address start = toAddr(Long.decode(args[i]));
				long len = Long.decode(args[i + 1]);
				Address end = start.add(len - 1);
				new DisassembleCommand(start, null, true).applyTo(currentProgram, monitor);
				pw.printf("## `%s` length `%s`%n%n", args[i], args[i + 1]);
				pw.println("```asm");
				Instruction ins = getInstructionAt(start);
				if (ins == null) {
					ins = getInstructionAfter(start.subtract(1));
				}
				while (ins != null && ins.getAddress().compareTo(end) <= 0) {
					StringBuilder operands = new StringBuilder();
					for (int op = 0; op < ins.getNumOperands(); op++) {
						if (op > 0) {
							operands.append(", ");
						}
						operands.append(ins.getDefaultOperandRepresentation(op));
					}
					pw.printf("%s  %-12s %s%n", ins.getAddress(), ins.getMnemonicString(), operands.toString());
					ins = getInstructionAfter(ins.getAddress());
				}
				pw.println("```");
				pw.println();
			}
		}
	}
}
