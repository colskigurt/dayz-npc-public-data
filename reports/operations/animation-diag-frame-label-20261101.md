# 진단 클라이언트 시계·희소 HUD 프레임·화면 투영 연결

작성: 2026-11-01T18:44:17.397539+09:00

## 1. 작업 목표

색상 detector의 머리/어깨 혼동을 우회할 수 있도록 network 진단 client의 HUD frame ID·실제 시계·개체변환·화면투영 기준을 수집한다. production NPC·애니메이션 시계를 보정하거나 기존 게임 PBO를 바꾸지 않는다. 추가 본 getter로 lazy cache 갱신을 강제하지 않는다.

## 2. 실제 수행

기존 도구로 작은 계측 전용 Mission 모듈을 별도 제작했다. 진단 빌드·명시 profile flag·network client에만 활성화하며 DGSubsetIntegratedCrowdLeader 하나를 찾는다. 매 callback에 frame ID, appActive, network ID, 개체 transform, camera 위치/방향, 고정8개 model reference의 engine screen projection을 기록한다. 본/애니메이션 getter·개체 위치쓰기·시뮬레이션 변경은 없다.

기존 정적 registry 분석을 A500의 자유함수 등록 규약(RDX name/R8 handler)에 맞춰 TickCount를 찾았다. method규약 첫 조회 결과0건을 보존하고 자유함수 handler308CD0→getter239EB0, QPC IAT13684F0·원점1A49D70·빈도1A49D68을 확인했다. 이 빌드에서 실제 TickCount(0) 반환을 QPC entry/exit bracket과 대조했다. REA 전체 decompilation/runtime Evidence가 아닌 기존 bounded Capstone·Python Frida 관찰이다.

시도1은 FileHandle에0을 대입한 컴파일 오류로 guard가 거부했다. 새 client·녹화·native 관찰은 시작되지 않았다. FileHandle이 typedef int[]임을 확인하고 null 대입으로 바꾼 v2를 별도 보존했다. 시도2는 컴파일됐지만 Widget.Show null VM 오류가 났다. client를 닫고 실패 영상/로그를 보존했으며 native는 연결하지 않았다. v3에서는 HUD 생성을 대상 준비 이후로 늦추고 매 bit를 root에서 직접 재조회·null 검사하며 READY를 첫 완성행 이후로 보냈다. 두 변경을 함께 적용했으므로 위젯 수명의 단일 원인이 확정됐다고 하지 않는다.

시도3은 client 실행·native15초·창 전용 무음12초녹화를 진행했다. CRF12로 파일을 제한했고 이전 CRF0와 화질/성능 비교에서 제외했다. 녹화 후 portable basic/scene/encoder의 원래bytes를 복원했다. HUD 텍스트는 실제 화면에서 확인했으나24개 binary Panel 막대는 보이지 않았다.9개 고정 영상 index에서 HUD 글자를 직접 읽어 로그와 연결했다. 차기 v4는 vanilla의style Colorable을 명시하는 layout만 준비했고 아직 build/실행하지 않았다.

## 3. 측정 결과

새 runtime3회: 컴파일 실패1·HUD VM 실패1·최종 계측 실행 성공1이다. 마지막 native435표본, scheduler/buffer/consumer/interpolation 각435회, cache1,212회(dirty746·clean466), observer/script오류0이다. TickCount(0)872회 모두 engine 원점+반환값이 host QPC bracket 안에 들었다. graph clock은 여전히 미확보다.

client frame log2,234행은 frame ID가 순차적이고 network ID/type은 하나다.436개행의 begin/end 양쪽 TickCount가 native hook과 직접 일치했다. HUD를 읽은9장 중8장이 이 직접 bracket 구간에 포함됐다.9장 모두appActive=true이며 전체행의 활성 여부는 별도JSON 집계를 보존한다. HUD문자만 희소 수동 검증했으며 전체 영상 game frame decode 성공은 아니다.

영상723프레임 모두 OBS출력표식/QPC에 연결됐다. 표식중복5건은 game drop으로 단정하지 않는다. 새 성공 영상105922872bytes·SHA694b33578504180f48770559ef5cbb396d502f39f73fb7f348050c347e2a2211이다.9개 HUD의 callback완료→OBS marker 간격 p50/p95/p99=25.8501/47.88284/50.719928ms다. 이는 물리 scanout·NPC render latency가 아니며9개 희소표본의 output-age 관찰이다.

8고정 model reference 중6개로 계산한 projective matrix를 다른2개에서 검사한 오차 p50/p95/p99=0.00003755/0.00011544/0.00011756px다. 실제 engine projection의 수치적 자기일관성이고 렌더된 vertex 위치의 독립 검증은 아니다.8장에는 직전 자연 Head MS cache를 화면에 투영한 검토 그림도 보존했다.9장 중첫장은 native 시작 전이므로 cache projection이 없고 임의로 채우지 않았다.

scheduler callback 간격 p50/p95/p99=23.08915/55.88782/61.313229ms다. HUD 위젯 조회/즉시업데이트·파일 기록·hook·OBS의 부하가 섞여 성능 baseline이나20ms p95 달성 여부에 쓰지 않는다. 실제 game/display frame-time, 새8/64 회귀·14clip 품질·GPU/할당/GC는 미측정이다.

## 4. 원인 분석

확인된 사실: 실제 진단 TickCount를 QPC에 직접 연결했고, HUD 텍스트9개와 해당 log ID를 대응시켰다. camera/모델 좌표를 engine projection 기준으로 화면에 옮기는 수치적 경로도 확보했다. 따라서 이전 실행의 empirical affine50ms를 엔진 지연으로 취급할 이유가 없다.

확인된 실패: 처음에는 핸들 형식 불일치가 compile을 막았고 v2는Widget.Show VM오류였다. v3는 해당 VM오류가 없지만 binary Panel은 보이지 않는다. 로컬 vanilla layout의 Colorable 스타일 누락을 후보로 잡았을 뿐 아직 원인 확정/해결 완료는 아니다. v4 script는v3와byte동일하고 layout에style25개만 추가했다.

미확인: binary막대의 완전 렌더·자동decode, 실제 game draw에 사용된 포즈 식별자, WGC texture 생성시각, 물리 scanout, 실제14동작 부드러움이다. HUD갱신·8reference projection·hook/recording의 추가비용은 별도 귀속하지 못했다. 즉시UI갱신 오버헤드는 가설이며 이번에 production 최적화로 반영하지 않는다.

## 5. 검증 수준

진단성공: 실제client 스크립트 실행,872개 clock bracket,9개 수동HUD 대응,고정기준점 projection 수치 검증. 미완료: 전체 자동frame identification·pose-to-render/display 시간 판정·시각 품질. 기능회귀/반복검증완료/최종수용은 아니다. 모듈 build시점의engineCompiled=false 영수증은 당시 상태로 보존하고 이후 실제compile/runtime 결과를 이번 보고서에서 별도 연결한다.

## 6. 변경 사항

새 계측PBO v1/v2/v3의 packed script/layout source 일치 검사와 자동 서명·검증은 모두 성공했다. v1/v2의 runtime 실패를 서명 성공과 혼동하지 않는다. 최종v3 PBO SHA=F6B12FB380994D43D4AEE906E319BB739756F3D69F8D5DF92677F9BD7740D182이다. 원래453D/0C6E/B8FB 세 PBO는 변경하지 않았고 마지막 실행의4개 signature 검증은 성공했다. 개인키는 배포물에 없다. v4는 source만 준비, PBO없음·실행미검증이다.

처음 AddonBuilder는 PBO를 만들었지만 PS5가 시작 stderr를 terminating error로 처리해 wrapper가 중단됐다. live builder가 없음을 확인하고 기존PBO의 packed member를 직접 검증한 뒤 서명했으며 중복 build하지 않았다. 후속build는 기존PS7에서 정상 완료했다. 이 운영실패도 보존한다. 운영서버·Steam 배포·기존NPC 움직임·ANM은 변경하지 않았다.

## 7. 원시 증거

묶음 animation-diag-frame-label-20261101-1825. source/v2/v3·build/packed proof/signature·runtime1/2/3의guard/launch/profile/crash·최종client-frame-label.tsv/native-observer/markers/decoded-markers/recorder 및수동HUD/QC그림·TickCount binding/clock-map을 보존한다. 최종 client 정상 종료 marker·강제종료없음·VM/crash0, serverDONE 뒤 소유server 종료를 확인했다. 실패2의 정상quit요청을 crash없음으로 보고하지 않는다. recorder정상종료 및3설정bytes복원을 확인했다.

공개본은 선정함수/집계/9개frame투영/한계를 제공하며 실제networkID·PID·CLI·절대runtime주소·로컬경로·원시영상·게임자산·키를 제외한다. 파일해시/JSON/종료검사 후 기존MD/TXT·진행JSON·B안 미러를 갱신한다. 원시보고서·실패본은 덮어쓰지 않는다.

## 8. 잔여 문제

다음은style만 바꾼v4를 별도서명한 뒤 checksum이 있는 막대가 실제로 칠해지는지 확인하는 것이다. 성공하면 같은client frameID를 전영상에서 decode해 native/cache/HUD 사이 순서를 확대한다. 실패하면 실제칠해지는vanilla HUD요소 또는기존텍스트decode를 사용하되 또다른추측을 main에 섞지 않는다.

위젯의Show(bool,immedUpdate)빈도/기록의 오버헤드는 다음에 따로 측정한다. style확인과동시에갱신정책을바꾸지 않는다. actor/frame/clock/projection 중하나라도불명확하면정확한latency판정을중단한다. 값싸게신뢰할수있는계측을확보하기전64개체성능비교·시간배율/새보간·프레임생략은보류한다.

선정 공개 본문. 로컬 보고서 SHA-256: `42e50b97f5f28807a0732da209ccd5603cca29943a17b6fc3d1ac8e67ea1a008`. 원시 전체가 아니다.
