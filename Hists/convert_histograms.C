// Suggested ranges based on typical values; adjust as needed
std::map<std::string, std::pair<double, double>> xRanges = {
	{"MuonEnergy", {0, 30}},	// MeV
	{"MuonStopZ", {-120, -70}}, // mm
	{"MuonStopTarget", {0, 5}}, // target indices
	{"MuonStopRadius", {0, 10}} // mm
};

void convert_histograms()
{
	TFile *f = TFile::Open("../muon_output.root");
	if (!f || f->IsZombie())
	{
		std::cerr << "Failed to open ROOT file." << std::endl;
		return;
	}

	const std::vector<std::string> histNames = {
		"MuonEnergy", "MuonStopZ", "MuonStopTarget", "MuonStopRadius"};

	for (const auto &name : histNames)
	{
		TH1D *hist = (TH1D *)f->Get(name.c_str());
		if (!hist)
		{
			std::cerr << "Histogram " << name << " not found." << std::endl;
			continue;
		}

		TCanvas *c = new TCanvas("c", "Canvas", 800, 600);

		// Apply axis range if specified
		if (xRanges.count(name))
		{
			hist->GetXaxis()->SetRangeUser(xRanges[name].first, xRanges[name].second);
		}

		hist->Draw();
		c->SaveAs((name + ".pdf").c_str());
		delete c;
	}

	f->Close();
}