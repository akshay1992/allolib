#include "al/util/ui/al_ControlGUI.hpp"

using namespace al;


void ControlGUI::draw(Graphics &g) {

    if (mManageIMGUI) {
        begin();
    }
//    ImGui::SetNextWindowSize(ImVec2(300, 450), ImGuiCond_FirstUseEver);
//    ImGui::SetNextWindowPos(ImVec2(mX, mY), ImGuiCond_FirstUseEver);
//    ImGui::Begin(std::to_string(mId).c_str());
    if (mNav) {
        if (ImGui::CollapsingHeader("Navigation", ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_DefaultOpen)) {
            Vec3d &currentPos = mNav->pos();
            float x = currentPos.elems()[0];

            bool changed = ImGui::SliderFloat("X", &x, -10, 10);
            if (changed) {
                currentPos.elems()[0] = x;
            }
            float y = currentPos.elems()[1];
            changed = ImGui::SliderFloat("Y", &y, -10, 10);
            if (changed) {
                currentPos.elems()[1] = y;
            }
            float z = currentPos.elems()[2];
            changed = ImGui::SliderFloat("Z", &z, -10, 10);
            if (changed) {
                currentPos.elems()[2] = z;
            }
        }
        ImGui::Spacing();
    }
    if (mPresetHandler) {
        if (ImGui::CollapsingHeader("Presets", ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_DefaultOpen)) {

            std::map<int, std::string> presets = mPresetHandler->availablePresets();
            static int selection = -1;
            std::string currentPresetName = mPresetHandler->getCurrentPresetName();

            ImGui::Text("%s", currentPresetName.c_str());

            int numColumns = 12;
            int numRows = 4;
            int counter = 0;
            for (int row = 0; row < numRows; row++) {
                for (int column = 0; column < numColumns; column++) {
                    std::string name = std::to_string(counter);
                    ImGui::PushID(counter);
                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1, 0.1, 0.1, 1.0));
                    ImGui::PopStyleColor(1);

                    bool is_selected = selection == counter;
                    if (ImGui::Selectable(name.c_str(), is_selected, 0, ImVec2(15, 15)))
                    {
                        if (mStoreButtonOn) {
                            mPresetHandler->storePreset(counter, name.c_str());
                            selection = counter;
                            mStoreButtonOn = false;
                        } else {
                            if (mPresetHandler->recallPreset(counter) != "") { // Preset is available
                                selection = counter;
                            }
                        }
                    }
                    if (column < numColumns - 1) ImGui::SameLine();
                    counter++;
                    ImGui::PopID();
                }
            }
            ImGui::Checkbox("Store", &mStoreButtonOn);
        }
    }
    if (mPolySynth) {
        if (ImGui::Button("Panic")) {
            mPolySynth->allNotesOff();
        }
    }
    if (mSynthRecorder) {
        static char buf1[64] = "test"; ImGui::InputText("Record Name", buf1, 64);
        if (ImGui::Checkbox("Record", &mRecordButtonValue)) {
            if (mRecordButtonValue) {
                mSynthRecorder->startRecord(buf1, mOverwriteButtonValue);
            } else {
                mSynthRecorder->stopRecord();
            }
        }
        ImGui::SameLine();
        if (ImGui::Checkbox("Overwrite", &mOverwriteButtonValue)) {

        }
    }
    if (mSynthSequencer) {
        if (ImGui::Button("Play")) {
            mSynthSequencer->playSequence("test.synthSequence");
        }
    }
//    ImGui::ShowDemoWindow();
    for (auto elem: mParameters) {
        if(elem.first == "" || ImGui::CollapsingHeader(elem.first.c_str(), ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_DefaultOpen)) { // ! to force open by default
            for (auto param: elem.second) {
                float value = param->get();
                bool changed = ImGui::SliderFloat(param->getName().c_str(), &value, param->min(), param->max());
                if (changed) {
                    param->set(value);
                }
            }

        }
    }
//    ImGui::End(); // End the window
    if (mManageIMGUI) {
        end();
    }
}

void ControlGUI::init(int x, int y) {
    static int id = 0;
    mId = id++;
    mX = x;
    mY = y;
    if (mManageIMGUI) {
        initIMGUI();
    }
}

void ControlGUI::begin() {
    string name = "__ControlGUI_" + std::to_string(mId);
    beginIMGUI_minimal(true, name.c_str(), mX, mY);
}

void ControlGUI::end() {
    endIMGUI_minimal(true);
}

void ControlGUI::cleanup() {
    if (mManageIMGUI) {
        shutdownIMGUI();
    }
}

ControlGUI &ControlGUI::registerParameter(Parameter &param) {
    std::string group = param.getGroup();
    if (mParameters.find(group) == mParameters.end()) {
        mParameters[group] = std::vector<Parameter *>();
    }
    mParameters[group].push_back(&param);
    return *this;
}

ControlGUI &ControlGUI::registerParameterBool(ParameterBool &param)
{
    std::string group = param.getGroup();
    if (mParameters.find(group) == mParameters.end()) {
        mParameters[group] = std::vector<Parameter *>();
    }
    mParameters[group].push_back(&param);
    return *this;
}

ControlGUI &ControlGUI::registerNav(Nav &nav)
{
    mNav = &nav;
    return *this;
}

ControlGUI &ControlGUI::registerPresetHandler(PresetHandler &presetHandler) {
    mPresetHandler = &presetHandler;
    return *this;
}

void ControlGUI::registerSynthRecorder(SynthRecorder &recorder) {
    mSynthRecorder = &recorder;
}

void ControlGUI::registerSynthSequencer(SynthSequencer &seq) {
    mSynthSequencer = &seq;
    mPolySynth = &seq.synth();
}
