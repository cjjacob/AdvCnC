import os
import ROOT
from subprocess import Popen, PIPE, call

datasets = ["DoubleEG", "SingleElectron", "SingleMuon", "JetHT"]

eos_base = "/eos/cms/store/group/phys_smp/cojacob/AdvCnC/"

def combine_root_files(sets):
    for dataset in sets:
        print "Combining " + dataset + " files..."
        list_of_files = []
        base_path = eos_base + dataset + os.sep
        for full_path, cur_dir, files in os.walk(base_path):
            print "Searching " + full_path
            if len(cur_dir)>0 and cur_dir[0]=="failed":
                continue
            for f in files:
                if f.endswith(".root"):
                    list_of_files.append(full_path + os.sep + f)
        command = ["hadd", "-f", "rAdvCnC_" + dataset + ".root"] + list_of_files
        process = Popen(command, stdout=PIPE, stderr=PIPE)
        stdout, stderr = process.communicate()
        print stdout
        print stderr
    return

def make_efficiencies_simple(sets):
    for dataset in sets:
        print "Working on " + dataset + "..." 

        tfile = ROOT.TFile("rAdvCnC_"+dataset+".root","read")
        tfile.cd()
        ROOT.gFile = tfile

        eem_pass = ROOT.TH3D()
        eem_fail = ROOT.TH3D()
        z_pass = ROOT.TH3D()
        z_fail = ROOT.TH3D()
        eem_p_je = ROOT.TH3D()
        eem_f_je = ROOT.TH3D()
        
        tfile.GetObject("/AdvCnC/Efficiency/EtaEtaMPass", eem_pass)
        tfile.GetObject("/AdvCnC/Efficiency/EtaEtaMFail", eem_fail)
        tfile.GetObject("/AdvCnC/Efficiency/ZPass", z_pass)
        tfile.GetObject("/AdvCnC/Efficiency/ZFail", z_fail)
        tfile.GetObject("/AdvCnC/JustEles/EtaEtaMPass_justEles", eem_p_je)
        tfile.GetObject("/AdvCnC/JustEles/EtaEtaMFail_justEles", eem_f_je)

        print "Got histos from rAdvCnC_"+dataset+".root"

        eem_p_proj = ROOT.TH2D()
        eem_f_proj = ROOT.TH2D()
        z_p_proj = ROOT.TH2D()
        z_f_proj = ROOT.TH2D()
        eem_p_je_proj = ROOT.TH2D()
        eem_f_je_proj = ROOT.TH2D()

        print "Doing projections... ",

        print "EtaEtaM ",
        eem_p_proj = eem_pass.Project3D("xy")
        eem_f_proj = eem_fail.Project3D("xy")
        eem_all_proj = eem_p_proj.Clone()
        eem_all_proj.Add(eem_f_proj)
        EtaEtaEff = ROOT.TEfficiency(eem_p_proj, eem_all_proj)

        print "ZPtEtaM ",
        z_p_proj = z_pass.Project3D("xy")
        z_f_proj = z_fail.Project3D("xy")
        z_all_proj = z_p_proj.Clone()
        z_all_proj.Add(z_f_proj)
        ZPtEtaEff = ROOT.TEfficiency(z_p_proj, z_all_proj)

        print "EtaEtaM_justEles"
        eem_p_je_proj = eem_p_je.Project3D("xy")
        eem_f_je_proj = eem_f_je.Project3D("xy")
        eem_a_je_proj = eem_p_je_proj.Clone()
        eem_a_je_proj.Add(eem_f_je_proj)
        EtaEtaJustElesEff = ROOT.TEfficiency(eem_p_je_proj, eem_a_je_proj)

        print "Writing TEfficiency histos... ",

        outfile = ROOT.TFile("eff_rAdvCnC_"+dataset+".root", "recreate")
        outfile.cd()
        ROOT.gFile = outfile
        EtaEtaEff.Write("EtaEtaEfficiency")
        ZPtEtaEff.Write("ZPtEtaEfficiency")
        EtaEtaJustElesEff.Write("EtaEtaJustElesEff")

        print "EtaEtaEff ",
        eee_canv = ROOT.TCanvas("EtaEtaEff_"+dataset, "EtaEtaEff_"+dataset, 800, 600)
        EtaEtaEff.Draw("colz text")
        eee_canv.Write("EtaEtaEff.png")

        print "ZPtEtaEff ", 
        zpe_canv = ROOT.TCanvas("ZPtEtaEff_"+dataset, "ZPtEtaEff_"+dataset, 800, 600)
        ZPtEtaEff.Draw("colz text")
        zpe_canv.Write("ZPtEtaEff.png")

        print "EtaEtaJEEff "
        je_canv = ROOT.TCanvas("EtaEtaJEEff_"+dataset, "EtaEtaJEEff_"+dataset, 800, 600)
        EtaEtaJustElesEff.Draw("colz text")
        je_canv.Write("EtaEtaJEEff.png")

        outfile.Close()
        print "Done!"

def main():
#    combine_root_files(datasets)
    make_efficiencies_simple(datasets)
    return

if __name__ == "__main__":
    main()
