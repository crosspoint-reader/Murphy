// Dumps bytes and printable ASCII around supplied addresses.

import java.io.File;
import java.io.PrintWriter;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.Memory;

public class DumpMemoryAt extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 3) {
			printerr("Usage: DumpMemoryAt <out.md> <length> <address> [address...]");
			return;
		}
		int length = Integer.decode(args[1]);
		Memory mem = currentProgram.getMemory();
		try (PrintWriter pw = new PrintWriter(new File(args[0]))) {
			pw.println("# Memory Dump");
			pw.println();
			for (int i = 2; i < args.length; i++) {
				Address start = toAddr(Long.decode(args[i]));
				pw.printf("## `%s`%n%n", args[i]);
				byte[] buf = new byte[length];
				int read = mem.getBytes(start, buf);
				for (int off = 0; off < read; off += 16) {
					int row = Math.min(16, read - off);
					pw.printf("`%s`  ", start.add(off));
					for (int j = 0; j < 16; j++) {
						if (j < row) {
							pw.printf("%02x ", buf[off + j] & 0xff);
						} else {
							pw.print("   ");
						}
					}
					pw.print(" ");
					for (int j = 0; j < row; j++) {
						int c = buf[off + j] & 0xff;
						pw.print(c >= 0x20 && c <= 0x7e ? (char)c : '.');
					}
					pw.println();
				}
				pw.println();
			}
		}
	}
}
