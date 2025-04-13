import ROOT

f = ROOT.TFile.Open("muon_output.root")
if not f or f.IsZombie():
    raise RuntimeError("Could not open ROOT file.")

hist_names = ["MuonEnergy", "MuonStopZ", "MuonStopTarget", "MuonStopRadius"]

for name in hist_names:
    hist = f.Get(name)
    if not hist:
        print(f"Histogram {name} not found.")
        continue

    canvas = ROOT.TCanvas("canvas", "", 800, 600)
    hist.Draw()
    canvas.SaveAs(f"{name}.pdf")