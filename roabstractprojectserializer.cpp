#include "roabstractprojectserializer.h"
#include <QDebug>

ROAbstractProjectSerializer::ROAbstractProjectSerializer(QObject *parent) :
    QObject(parent),
    _curElementType(NoToken),
    _curText("") { }

void ROAbstractProjectSerializer::deserialize(ROProject* const proj, QTextStream& ostream) {
    beginRead(ostream);
    if (_curElementType != StartDocument) return;
    if (readElementStart() && _curText == "Project") {
        if (!readElementStart()) return;

        if (_curText == "ProjectInfo") {
            ROProjectInfo* const info = proj->info();
            if (!readElement()) return;

            if (_curElementType == StartElement) {
                if (_curText == "name") {
                    if (!readElement()) return;
                    if (_curElementType == TextElement) {
                        info->setName(_curText);
                        if (!readElement()) return;
                    }
                    if (_curElementType != EndElement || _curText != "name") return;
                    if (!readElement()) return;
                }
                if (_curText == "author") {
                    if (!readElement()) return;
                    if (_curElementType == TextElement) {
                        info->setAuthor(_curText);
                        if (!readElement()) return;
                    }
                    if (_curElementType != EndElement || _curText != "author") return;
                    if (!readElement()) return;
                }
                if (_curText == "company") {
                    if (!readElement()) return;
                    if (_curElementType == TextElement) {
                        info->setCompany(_curText);
                        if (!readElement()) return;
                    }
                    if (_curElementType != EndElement || _curText != "company") return;
                    if (!readElement()) return;
                }
                if (_curText == "notes") {
                    if (!readElement()) return;
                    if (_curElementType == TextElement) {
                        info->setNotes(_curText);
                        if (!readElement()) return;
                    }
                    if (_curElementType != EndElement || _curText != "notes" || !readElement()) return;
                }
            }
            if (_curElementType != EndElement || _curText != "ProjectInfo" || !readElement()) return;
        }
        // OPTIONAL read cases
        if (_curText == "Cases") {

            // read case count
            if (!readElement() || _curElementType != StartElement || _curText != "caseCount") return;
            if (!readElement() || _curElementType != TextElement) return;
            bool convertSuccess;
            int caseCount = _curText.toInt(&convertSuccess);
            if (!convertSuccess || !readElement() || _curElementType != EndElement || _curText != "caseCount") return;
            proj->setCaseCount(caseCount);
            if (!readElement()) return;

            for (int cIdx=0; cIdx<proj->caseCount(); ++cIdx) {
                // Start case
                if (_curElementType != StartElement || _curText != "Case" || !readElement()) return;
                ROCase* case_  = proj->case_(cIdx);

                if (_curElementType == StartElement) {
                    // OPTIONAL read system
                    if (_curText == "System") {
                        // Start System
                        ROSystem* sys = case_->sys();
                        if (!readElement()) return;


                        // OPTIONAL read water type index
                        if (_curElementType == StartElement && _curText == "waterTypeIndex") {
                            if (!readElement()) return;
                            if (_curElementType == TextElement) {
                                int waterTypeIndex = _curText.toInt(&convertSuccess);
                                if (convertSuccess) sys->setWaterTypeIndex(waterTypeIndex);
                                if (!readElement()) return;
                            }
                            if (_curElementType != EndElement || _curText != "waterTypeIndex") return;
                            if (!readElement()) return;
                        }

                        // OPTIONAL element lifetime
                        if (_curElementType == StartElement && _curText == "elementLifetime") {
                            if (!readElement()) return;
                            if (_curElementType == TextElement) {
                                int elementLifetime = _curText.toInt(&convertSuccess);
                                if (convertSuccess) sys->setElementLifetime(elementLifetime);
                                if (!readElement()) return;
                            }
                            if (_curElementType != EndElement || _curText != "elementLifetime") return;
                            if (!readElement()) return;
                        }

                        // OPTIONAL read feeds data
                        if (_curElementType == StartElement && _curText == "Feeds") {
                            // read feeds count
                            if (!readElement() || _curElementType != StartElement || _curText != "feedCount" ||
                                    !readElement() || _curElementType != TextElement) return;
                            bool convertSuccess;
                            int feedCount = _curText.toInt(&convertSuccess);
                            if (!convertSuccess || !readElement() || _curElementType != EndElement || _curText != "feedCount") return;
                            sys->setFeedCount(feedCount);
                            if (!readElement()) return;

                            for (int fIdx=0; fIdx<sys->feedCount(); ++fIdx) {
                                // Start feed
                                if (_curElementType != StartElement || _curText != "Feed") return;
                                ROFlow* feed = fIdx < sys->feedCount() ? sys->partFeed(fIdx) : sys->addPartFeed();
                                if (!readElement()) return;

                                if (_curElementType == StartElement) {
                                    // OPTIONAL read part
                                    if (_curText == "part") {
                                        if (!readElement()) return;
                                        if (_curElementType == TextElement) {
                                            double part = _curText.toDouble(&convertSuccess);
                                            if (convertSuccess) feed->setPart(part);
                                            if (!readElement()) return;
                                        }
                                        if (_curElementType != EndElement || _curText != "part") return;
                                        if (!readElement()) return;
                                    }

                                    // OPTIONAL FLOW WRAPPER (deprecated since 1.2.4.5)
                                    if (_curText == "Flow")
                                        if (!readElement()) return;

                                    if (_curElementType == StartElement) {
                                        // OPTIONAL temperature
                                        if (_curText == "temperature") {
                                            if (!readElement()) return;
                                            if (_curElementType == TextElement) {
                                                double temperature = _curText.toDouble(&convertSuccess);
                                                    if (convertSuccess) feed->setTemperature(temperature);
                                                if (!readElement()) return;
                                            }
                                            if (_curElementType != EndElement || _curText != "temperature") return;
                                            if (!readElement()) return;
                                        }

                                        // OPTIONAL ph
                                        if (_curText == "pH") {
                                            if (!readElement()) return;
                                            if (_curElementType == TextElement) {
                                                double pH = _curText.toDouble(&convertSuccess);
                                                    if (convertSuccess) feed->setPH(pH);
                                                if (!readElement()) return;
                                            }
                                            if (_curElementType != EndElement || _curText != "pH") return;
                                            if (!readElement()) return;
                                        }

                                        // OPTIONAL solutes
                                        if (_curText == "Solutes") {
                                            if (!readElement()) return;
                                                ROSolutes* solutes = feed->solutes();
                                            solutes->beginChange();
                                            for (int si=0; si < ROSolutes::TotalIons; ++si) {
                                                QString soluteName = ROSoluteModel::shortNameByIndex(si);
                                                if (_curElementType != StartElement || _curText != soluteName ||
                                                        !readElement()) {
                                                    solutes->endChange();
                                                    return;
                                                }

                                                if (_curElementType == TextElement) {
                                                    double soluteMeql = _curText.toDouble(&convertSuccess);
                                                    if (convertSuccess) solutes->setMeql(si, soluteMeql);
                                                    if (!readElement()) {
                                                        solutes->endChange();
                                                        return;
                                                    }
                                                }
                                                if (_curElementType != EndElement || _curText != soluteName ||
                                                        !readElement()) {
                                                    solutes->endChange();
                                                    return;
                                                }
                                            }
                                            solutes->endChange();
                                            if (_curElementType != EndElement || _curText != "Solutes" ||
                                                    !readElement()) return;
                                        }
                                    }
                                    // OPTIONAL FLOW CLOSED
                                    if (_curElementType == EndElement && _curText == "Flow")
                                        if (!readElement()) return;
                                }
                                if (_curElementType != EndElement || _curText != "Feed" ||
                                        !readElement()) return;
                            }
                            if (_curElementType != EndElement || _curText != "Feeds" ||
                                    !readElement()) return;
                        }

                        if (_curElementType == StartElement && _curText == "Passes") {
                            // read feeds count
                            if (!readElement() || _curElementType != StartElement || _curText != "passCount" ||
                                    !readElement() || _curElementType != TextElement) return;
                            bool convertSuccess;
                            int passCount = _curText.toInt(&convertSuccess);
                            if (!convertSuccess || !readElement() || _curElementType != EndElement || _curText != "passCount") return;
                            sys->setPassCount(passCount);
                            if (!readElement()) return;

                            for (int pi=0; pi<sys->passCount(); ++pi) {
                                // Start pass
                                if (_curElementType != StartElement || _curText != "Pass") return;
                                ROPass* pass = pi < sys->passCount() ? sys->pass(pi) : sys->addPass();
                                if (!readElement()) return;

                                // OPTIONAL permeate flow rate
                                if (_curElementType == StartElement && _curText == "permeateFlowRate") {
                                    if (!readElement()) return;
                                    if (_curElementType == TextElement) {
                                        double permeateFlowRate = _curText.toDouble(&convertSuccess);
                                        if (convertSuccess) pass->permeate()->setRate(permeateFlowRate);
                                        if (!readElement()) return;
                                    }
                                    if (_curElementType != EndElement || _curText != "permeateFlowRate") return;
                                    if (!readElement()) return;
                                }

                                // OPTIONAL recovery
                                if (_curElementType == StartElement && _curText == "recovery") {
                                    if (!readElement()) return;
                                    if (_curElementType == TextElement) {
                                        double recovery = _curText.toDouble(&convertSuccess);
                                        if (convertSuccess) pass->setRecovery(recovery);
                                        if (!readElement()) return;
                                    }
                                    if (_curElementType != EndElement || _curText != "recovery") return;
                                    if (!readElement()) return;
                                }

                                // OPTIONAL feed flow rate
                                if (_curElementType == StartElement && _curText == "feedFlowRate") {
                                    if (!readElement()) return;
                                    if (_curElementType == TextElement) {
                                        double feedFlowRate = _curText.toDouble(&convertSuccess);
                                        if (convertSuccess) pass->feed()->setRate(feedFlowRate);
                                        if (!readElement()) return;
                                    }
                                    if (_curElementType != EndElement || _curText != "feedFlowRate") return;
                                    if (!readElement()) return;
                                }

                                // OPTIONAL flow factor
                                if (_curElementType == StartElement && _curText == "flowFactor") {
                                    // не используется в новой версии, т.к. теперь зависит от типа воды

                                    // просто чтение далее с проверкой
                                    if (!readElement() || !readElement()) return;
                                    if (_curElementType != EndElement || _curText != "flowFactor") return;
                                    if (!readElement()) return;
                                }

                                // OPTIONAL back Pressure
                                if (_curElementType == StartElement && _curText == "backPressure") {
                                    if (!readElement()) return;
                                    if (_curElementType == TextElement) {
                                        double backPressure = _curText.toDouble(&convertSuccess);
                                        if (convertSuccess) pass->setBackPressure(backPressure);
                                        if (!readElement()) return;
                                    }
                                    if (_curElementType != EndElement || _curText != "backPressure") return;
                                    if (!readElement()) return;
                                }

                                // OPTIONAL Self Recycle
                                if (_curElementType == StartElement && _curText == "selfRecycle") {
                                    if (!readElement()) return;
                                    if (_curElementType == TextElement) {
                                        double selfRecycle = _curText.toDouble(&convertSuccess);
                                        if (convertSuccess) {
                                            pass->setSelfRecycle(selfRecycle);
                                            pass->setHasSelfRecycle(true);
                                        }
                                        if (!readElement()) return;
                                    }
                                    if (_curElementType != EndElement || _curText != "selfRecycle") return;
                                    if (!readElement()) return;
                                }

                                // OPTIONAL Blend Permeate
                                if (_curElementType == StartElement && _curText == "blendPermeate") {
                                    if (!readElement()) return;
                                    if (_curElementType == TextElement) {
                                        double blendPermeate = _curText.toDouble(&convertSuccess);
                                        if (convertSuccess) {
                                            pass->setBlendPermeate(blendPermeate);
                                            sys->setHasBlendPermeate(true);
                                        }
                                        if (!readElement()) return;
                                    }
                                    if (_curElementType != EndElement || _curText != "blendPermeate") return;
                                    if (!readElement()) return;
                                }

                                if (_curText == "Stages") {
                                    // read stage count
                                    if (!readElement() || _curElementType != StartElement || _curText != "stageCount" ||
                                            !readElement() || _curElementType != TextElement) return;
                                    bool convertSuccess;
                                    int stageCount = _curText.toInt(&convertSuccess);
                                    if (!convertSuccess || !readElement() || _curElementType != EndElement || _curText != "stageCount") return;
                                    pass->setStageCount(stageCount);
                                    if (!readElement()) return;

                                    for (int si=0; si<pass->stageCount(); ++si) {
                                        // Start stage
                                        if (_curElementType != StartElement || _curText != "Stage") return;
                                        ROStage* stage = si < pass->stageCount() ? pass->stage(si) : pass->addStage();
                                        if (!readElement()) return;

                                        // OPTIONAL vessel Count
                                        if (_curElementType == StartElement && _curText == "vesselCount") {
                                            if (!readElement()) return;
                                            if (_curElementType == TextElement) {
                                                int vesselCount = _curText.toInt(&convertSuccess);
                                                if (convertSuccess) stage->setVesselCount(vesselCount);
                                                if (!readElement()) return;
                                            }
                                            if (_curElementType != EndElement || _curText != "vesselCount") return;
                                            if (!readElement()) return;
                                        }

                                        // OPTIONAL elements Per Vessel Count
                                        if (_curElementType == StartElement && _curText == "elementsPerVesselCount") {
                                            if (!readElement()) return;
                                            if (_curElementType == TextElement) {
                                                int elementsPerVesselCount = _curText.toInt(&convertSuccess);
                                                if (convertSuccess) stage->setElementsPerVesselCount(elementsPerVesselCount);
                                                if (!readElement()) return;
                                            }
                                            if (_curElementType != EndElement || _curText != "elementsPerVesselCount") return;
                                            if (!readElement()) return;
                                        }

                                        // OPTIONAL pre Stage Pressure
                                        if (_curElementType == StartElement && _curText == "preStagePressure") {
                                            if (!readElement()) return;
                                            if (_curElementType == TextElement) {
                                                double preStagePressure = _curText.toDouble(&convertSuccess);
                                                if (convertSuccess) stage->setPreStagePressure(preStagePressure);
                                                if (!readElement()) return;
                                            }
                                            if (_curElementType != EndElement || _curText != "preStagePressure") return;
                                            if (!readElement()) return;
                                        }

                                        // начиная с версии 1.4.1 давление фильтрата является параметром ступени, а не стадии
                                        // для поддержки совместимости на стадии оно просто пропускается
                                        if (_curElementType == StartElement && _curText == "backPressure") {
                                            readElement();
                                            readElement();
                                            readElement();
                                        }


                                        // OPTIONAL membrane Index
                                        if (_curElementType == StartElement && _curText == "membraneId") {
                                            if (!readElement()) return;
                                            if (_curElementType == TextElement) {
                                                int membraneId = _curText.toInt(&convertSuccess);
                                                if (convertSuccess) stage->setMembraneId(membraneId);
                                                if (!readElement()) return;
                                            }
                                            if (_curElementType != EndElement || _curText != "membraneId") return;
                                            if (!readElement()) return;
                                        }

                                        // начиная с версии 1.2.1 используется id элемента, а не индекс
                                        // для поддержки совместимости индекс просто пропускается
                                        if (_curElementType == StartElement && _curText == "membraneIndex") {
                                            readElement();
                                            readElement();
                                            readElement();
                                        }

                                        if (_curElementType != EndElement || _curText != "Stage" ||
                                                !readElement()) return;
                                    }
                                    if (_curElementType != EndElement || _curText != "Stages" ||
                                            !readElement()) return;
                                }
                                if (_curElementType != EndElement || _curText != "Pass" ||
                                        !readElement()) return;
                            }
                            if (_curElementType != EndElement || _curText != "Passes" ||
                                    !readElement()) return;
                        }

                        if (_curElementType == StartElement && _curText == "Recycles") {
                            // read feeds count
                            if (!readElement() || _curElementType != StartElement || _curText != "recycleCount" ||
                                    !readElement() || _curElementType != TextElement) return;
                            bool convertSuccess;
                            int recycleCount = _curText.toInt(&convertSuccess);
                            if (!convertSuccess || !readElement() || _curElementType != EndElement || _curText != "recycleCount") return;
                            if (!readElement()) return;

                            for (int ri=0; ri<recycleCount; ++ri) {
                                // Start recycle
                                if (_curElementType != StartElement || _curText != "Recycle" ||
                                        !readElement()) return;

                                // read from pass
                                if (_curElementType != StartElement || _curText != "fromPass" ||
                                        !readElement() || _curElementType != TextElement) return;
                                int fromPass = _curText.toInt(&convertSuccess);
                                if (!convertSuccess ||
                                        !readElement() || _curElementType != EndElement || _curText != "fromPass" ||
                                        !readElement()) return;

                                // read to pass
                                if (_curElementType != StartElement || _curText != "toPass" ||
                                        !readElement() || _curElementType != TextElement) return;
                                int toPass = _curText.toInt(&convertSuccess);
                                if (!convertSuccess ||
                                        !readElement() || _curElementType != EndElement || _curText != "toPass" ||
                                        !readElement()) return;

                                // read recycle rate
                                if (_curElementType != StartElement || _curText != "rate" ||
                                        !readElement() || _curElementType != TextElement) return;
                                double recycleRate = _curText.toDouble(&convertSuccess);
                                if (!convertSuccess ||
                                        !readElement() || _curElementType != EndElement || _curText != "rate") return;

                                sys->pass(fromPass)->addRecycle(toPass, recycleRate);
                                if (!readElement() || _curElementType != EndElement || _curText != "Recycle" ||
                                        !readElement()) return;
                            }

                            if (_curElementType != EndElement || _curText != "Recycles" ||
                                    !readElement()) return;

                        }

                        if (_curElementType == StartElement && _curText == "Scaling") {
                            if (!readElement()) return;
                            ROScalingElement* se = sys->scalingElement();

                            if (_curElementType == StartElement && _curText == "feedAdjustment") {
                                if (!readElement() || _curElementType != TextElement) return;
                                ROScalingElement::FeedAdjustment feedAdjustment = static_cast<ROScalingElement::FeedAdjustment>(_curText.toInt(&convertSuccess));
                                if (!convertSuccess ||
                                        !readElement() || _curElementType != EndElement || _curText != "feedAdjustment" || !readElement()) return;
                                se->setAdjustment(feedAdjustment);

                            }

                            //

                            bool caLeakageExplicit = false;
                            if (_curElementType == StartElement && _curText == "caLeakageExplicit") {
                                if (!readElement() || _curElementType != TextElement) return;
                                caLeakageExplicit = _curText.toInt(&convertSuccess);
                                if (!convertSuccess ||
                                        !readElement() || _curElementType != EndElement || _curText != "caLeakageExplicit" || !readElement()) return;

                            }

                            if (_curElementType != StartElement || _curText != "caLeakage" ||
                                    !readElement() || _curElementType != TextElement) return;
                            double caLeakage = _curText.toDouble(&convertSuccess);
                            if (!convertSuccess ||
                                    !readElement() || _curElementType != EndElement || _curText != "caLeakage") return;
                            if (caLeakageExplicit) se->setCaLeakage(caLeakage);
                            else se->updateCaLeakage(caLeakage);

                            bool mgLeakageExplicit = false;
                            if (readElement() && _curElementType == StartElement && _curText == "mgLeakageExplicit") {
                                if (!readElement() || _curElementType != TextElement) return;
                                mgLeakageExplicit = _curText.toInt(&convertSuccess);
                                if (!convertSuccess ||
                                        !readElement() || _curElementType != EndElement || _curText != "mgLeakageExplicit" || !readElement()) return;

                            }

                            if (_curElementType != StartElement || _curText != "mgLeakage" ||
                                    !readElement() || _curElementType != TextElement) return;
                            double mgLeakage = _curText.toDouble(&convertSuccess);
                            if (!convertSuccess ||
                                    !readElement() || _curElementType != EndElement || _curText != "mgLeakage") return;
                            if (mgLeakageExplicit) se->setMgLeakage(mgLeakage);
                            else se->updateMgLeakage(mgLeakage);


                            if (!readElement() || _curElementType != StartElement || _curText != "targetPh" ||
                                    !readElement() || _curElementType != TextElement) return;
                            double targetPh = _curText.toDouble(&convertSuccess);
                            if (!convertSuccess ||
                                    !readElement() || _curElementType != EndElement || _curText != "targetPh") return;
                            se->setTargetPh(targetPh);

                            if (!readElement() || _curElementType != StartElement || _curText != "acidConcentration" ||
                                    !readElement() || _curElementType != TextElement) return;
                            double acidConcentration = _curText.toDouble(&convertSuccess);
                            if (!convertSuccess ||
                                    !readElement() || _curElementType != EndElement || _curText != "acidConcentration") return;
                            se->setAcidConcentration(acidConcentration);

                            if (!readElement() || _curElementType != StartElement || _curText != "dosingAcid" ||
                                    !readElement() || _curElementType != TextElement) return;
                            int dosingAcid = _curText.toInt(&convertSuccess);
                            if (!convertSuccess ||
                                    !readElement() || _curElementType != EndElement || _curText != "dosingAcid") return;
                            se->setDosingAcid(static_cast<ROScalingElement::Acid>(dosingAcid));

                            if (!readElement() || _curElementType != EndElement || _curText != "Scaling" ||
                                    !readElement()) return;
                        }

                        if (_curElementType != EndElement || _curText != "System" ||
                                !readElement()) return;
                    }



                    if (_curText == "SystemController") {
                        ROSystemController* sysC = case_->sysC();
                        if (!readElement() || _curElementType != StartElement || _curText != "passCCount" ||
                                !readElement() || _curElementType != TextElement) return;
                        bool convertSuccess;
                        int passCCount = _curText.toInt(&convertSuccess);
                        if (!convertSuccess || passCCount != case_->sys()->passCount() ||
                                !readElement() || _curElementType != EndElement || _curText != "passCCount" ||
                                !readElement()) return;

                        for (int pci=0; pci < sysC->passCCount(); ++pci) {
                            if (_curElementType != StartElement || _curText != "PassController" ||
                                    !readElement()) return;
                            ROPassController* passC = sysC->passC(pci);
                            passC->reset();

                            // OPTIONAL permeate Set State
                            if (_curElementType == StartElement && _curText == "permeateSetState") {
                                if (passC->paramSetC()->permeateSetState() == ROPassParamSetController::ParamSetUndefined)
                                    passC->paramSetC()->setPermeateSetState(ROPassParamSetController::ParamSetExplicit);
                                if (!readElement() || _curElementType != EndElement || _curText != "permeateSetState" ||
                                        !readElement()) return;
                            }

                            // OPTIONAL recovery Set State
                            if (_curElementType == StartElement && _curText == "recoverySetState") {
                                if (passC->paramSetC()->recoverySetState() == ROPassParamSetController::ParamSetUndefined)
                                    passC->paramSetC()->setRecoverySetState(ROPassParamSetController::ParamSetExplicit);
                                if (!readElement() || _curElementType != EndElement || _curText != "recoverySetState" ||
                                        !readElement()) return;
                            }

                            // OPTIONAL feed Set State
                            if (_curElementType == StartElement && _curText == "feedSetState") {
                                if (passC->paramSetC()->feedSetState() == ROPassParamSetController::ParamSetUndefined)
                                    passC->paramSetC()->setFeedSetState(ROPassParamSetController::ParamSetExplicit);
                                if (!readElement() || _curElementType != EndElement || _curText != "feedSetState" ||
                                        !readElement()) return;
                            }

                            if (_curElementType != EndElement || _curText != "PassController" ||
                                    !readElement()) return;
                        }
                        if (_curElementType != EndElement || _curText != "SystemController" ||
                                !readElement()) return;
                    }
                } // CASES
                if (_curElementType != EndElement || _curText != "Case" ||
                        !readElement()) return;
            }
            if (_curElementType != EndElement || _curText != "Cases" ||
                    !readElement()) return;
        }

        if (_curElementType != EndElement || _curText != "Project" ||
                !readElement()) return;
    }
    endRead();
    //return endRead();
}

bool ROAbstractProjectSerializer::serialize(const ROProject *const proj, QTextStream& istream) {
#define int2Str(val) QString::number(val)
#define double2Str(val) QString::number(val, 'g', 15)
    beginWrite(istream);
    writeStartElement("Project"); {
        const ROProjectInfo* const info = proj->info();
        writeStartElement("ProjectInfo"); {
            writeElement("name", info->name());
            writeElement("author", info->author());
            writeElement("company", info->company());
            writeElement("notes", info->notes());
        } writeEndElement(); // ProjectInfo
        writeStartElement("Cases"); {
            writeElement("caseCount", int2Str(proj->caseCount()));
            for (int cIdx=0; cIdx < proj->caseCount(); ++cIdx) {
                writeStartElement("Case"); {
                    ROCase* case_ = proj->case_(cIdx);
                    writeStartElement("System"); {
                        ROSystem* sys = case_->sys();
                        writeElement("waterTypeIndex", int2Str(sys->waterTypeIndex()));
                        writeElement("elementLifetime", int2Str(sys->elementLifetime()));
                        writeStartElement("Feeds"); {
                            writeElement("feedCount", int2Str(sys->feedCount()));
                            for (int fIdx=0; fIdx < sys->feedCount(); ++fIdx) {
                                writeStartElement("Feed"); {
                                    ROFlow* feed = sys->partFeed(fIdx);
                                    writeElement("part", double2Str(feed->part()));
                                    writeElement("temperature", double2Str(feed->temperature()));
                                    writeElement("pH", double2Str(feed->pH()));
                                    ROSolutes* solutes = feed->solutes();
                                    writeStartElement("Solutes"); {
                                        for (int sIdx=0; sIdx < ROSolutes::TotalIons; ++sIdx) {
                                            writeElement(ROSoluteModel::shortNameByIndex(sIdx), double2Str(solutes->meql(sIdx)));
                                        }
                                    } writeEndElement(); // Solutes
                                } writeEndElement(); // Feed
                            }
                        } writeEndElement(); // Feeds

                        writeStartElement("Passes"); {
                            writeElement("passCount", int2Str(sys->passCount()));
                            for (int pIdx=0; pIdx < sys->passCount(); ++pIdx) {
                                writeStartElement("Pass"); {
                                    ROPass* pass = sys->pass(pIdx);

                                    writeElement("permeateFlowRate", double2Str(pass->permeate()->rate()));
                                    writeElement("recovery", double2Str(pass->recovery()));
                                    writeElement("feedFlowRate", double2Str(pass->feed()->rate()));
                                    writeElement("flowFactor", double2Str(pass->flowFactor()));
                                    writeElement("backPressure", double2Str(pass->backPressure()));
                                    if (pass->hasSelfRecycle()) writeElement("selfRecycle", double2Str(pass->selfRecycle()));
                                    if (pass->hasBlendPermeate()) writeElement("blendPermeate", double2Str(pass->blendPermeate()));

                                    writeStartElement("Stages"); {
                                        writeElement("stageCount", int2Str(pass->stageCount()));
                                        for (int sIdx=0; sIdx < pass->stageCount(); ++sIdx) {
                                            writeStartElement("Stage"); {
                                                ROStage* stage = pass->stage(sIdx);
                                                writeElement("vesselCount", int2Str(stage->vesselCount()));
                                                writeElement("elementsPerVesselCount", int2Str(stage->elementsPerVesselCount()));
                                                writeElement("preStagePressure", double2Str(stage->preStagePressure()));
                                                writeElement("membraneId", int2Str(stage->membraneId()));
                                            } writeEndElement(); // Stage
                                        }
                                    } writeEndElement(); // Stages
                                } writeEndElement(); // Pass
                            }
                        } writeEndElement(); // Passes

                        writeStartElement("Recycles"); {
                            writeElement("recycleCount", int2Str(sys->totalRecycleCount()));
                            for (int pIdx=0; pIdx<sys->passCount(); ++pIdx) {
                                for (int pIdx2=0; pIdx2<pIdx; ++pIdx2) {
                                    if (sys->hasRecycle(pIdx, pIdx2)) {
                                        writeStartElement("Recycle"); {
                                            writeElement("fromPass", int2Str(pIdx));
                                            writeElement("toPass", int2Str(pIdx2));
                                            //double val = sys->recycle(pIdx, pIdx2);
                                            writeElement("rate", double2Str(sys->recycle(pIdx, pIdx2)));
                                        } writeEndElement(); // Recycle
                                    }
                                }
                            }
                        } writeEndElement(); // Recycles

                        writeStartElement("Scaling"); {
                            ROScalingElement* se = sys->scalingElement();
                            writeElement("feedAdjustment", int2Str(se->adjustment()));
                            writeElement("caLeakageExplicit", int2Str(se->caLeakageExplicit()));
                            writeElement("caLeakage", double2Str(se->caLeakage()));
                            writeElement("mgLeakageExplicit", int2Str(se->mgLeakageExplicit()));
                            writeElement("mgLeakage", double2Str(se->mgLeakage()));
                            writeElement("targetPh", double2Str(se->targetPh()));
                            writeElement("acidConcentration", double2Str(se->acidConcentration()));
                            writeElement("dosingAcid", int2Str(se->dosingAcid()));
                        } writeEndElement(); // Recycles

                    } writeEndElement(); // System

                    writeStartElement("SystemController"); {
                        ROSystemController* sysC = case_->sysC();
                        writeElement("passCCount", int2Str(sysC->passCCount()));
                        for (int pcIdx=0; pcIdx < sysC->passCCount(); ++pcIdx) {
                            writeStartElement("PassController"); {
                                ROPassController* passC = sysC->passC(pcIdx);
                                if (passC->paramSetC()->permeateSetState() == ROPassParamSetController::ParamSetExplicit)
                                    writeElement("permeateSetState", "");
                                if (passC->paramSetC()->recoverySetState() == ROPassParamSetController::ParamSetExplicit)
                                    writeElement("recoverySetState", "");
                                if (passC->paramSetC()->feedSetState() == ROPassParamSetController::ParamSetExplicit)
                                    writeElement("feedSetState", "");
                            } writeEndElement(); // PassController
                        }
                    } writeEndElement(); // SystemController
                } writeEndElement(); // Case
            }
        } writeEndElement(); // Cases
    } writeEndElement(); // Project
    return endWrite();
}

bool ROAbstractProjectSerializer::endOfElement() const { return _curElementType == EndDocument || _curElementType == EndElement || _curElementType == Invalid; }

bool ROAbstractProjectSerializer::readCheckNestedText(const QString& name) {
    return (_curText == name && readNestedText());
}

bool ROAbstractProjectSerializer::readNestedText() { return (readElement() && _curElementType == TextElement); }

bool ROAbstractProjectSerializer::readRequiredElement(const QString &name) { return readElementStart() && _curText == name; }

bool ROAbstractProjectSerializer::readElementStart() { return readElement() && _curElementType == StartElement; }

bool ROAbstractProjectSerializer::readElementEnd() { return readElement() && _curElementType == EndElement; }
