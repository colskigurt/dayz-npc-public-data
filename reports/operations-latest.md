# 물리 질의 마스크 검증과 기각 후보 중복 제외

## 1. 작업 목표
서버 LOD의 클라이언트 포즈 gate 실패 후, 기존 물리 질의 비용 연구에서 미검증 수정 지점을 선별한다. 정상 밀침·낙하·착지·FOLLOW·재등반을 보존하고, 이미 기각된 후보를 반복하지 않는다.

## 2. 실제 수행
현행 DGBGroundAgent.Floors/Walls, SolveContactsInternal, Crowd.Displace와 기존 support-context·terrain-negative-backoff·후처리0변위·물리API 경계 보고서를 읽었다. 공식 DayZ-Script-Diff의 API 선언도 확인했다. 기존10개 PBO와 현행 서버 해시를 고정한 무개체 headless 미션에서 enum 상수와 실제 조합값만 출력했다. 클라이언트/네이티브 NPC0, 물리 질의·이동·상태 setter0이다.
DESIGN.md의 최초 상태와 실제 후속 family-disposition을 구분했다. terrain-negative-backoff는 설계만 남은 후보가 아니라8/16회 변형의 국소 gate가 실패해 가족 확장이 중단된 후보다. 오래된 설명을 근거로 다시 구현하지 않는다.

## 3. 측정 결과
현행 서버 runtime:Floors=33064, Walls=67408168. BUILDING8·VEHICLE32·ROADWAY256·TERRAIN32768의OR가Floors와 정확히 일치했다. FENCE262144·DOOR4096·ITEM_LARGE67108864를 더한OR가Walls와 일치했다. AI524288·CHARACTER16·DYNAMICITEM64 비트는Walls에 없다. 마스크 추가 비트0이다. 요청 종료 뒤 실제exit−1·강제종료 없음, 완료 표식/스크립트 오류 없음·프로세스 소멸 확인. p50/p95/p99·CPU·실제 시각을 새로 측정하지 않았다.

## 4. 원인 분석
확인:현재 비용을 잘못 조합된Floors/Walls 마스크 탓으로 설명할 근거가 없다. 이 런타임 상수값을 일반 정수 enum 순번으로 추측하지 않는다. API 선언의 입력에는 geometry/layer/ignore와결과만 있고 공개 scratch/batch 인자는 없다. RV 계열은 다른 교차 형식·결과 계약이며 같은 의미/낮은 비용이 증명되지 않았다.
기존 후보 판정:

| 후보 | 실제 상태·중복 근거 | 이번 결정 |
|---|---|---|
| 서버 follower LOD | 활성 클라이언트 공통 본 최대0.261m, 포즈 gate 실패 | 확대 금지 |
| SupportSweepFast 상태 제한 | 현행 서버 build26220720 cost64-three2에서 서버+5.19%·Present+7.12% | 이름만 바꾼 guard 재시험 제외 |
| Scratch/스냅샷 | 이전 국소 미달/실제 메서드 회귀 | 재구현 제외 |
| Drive/Displace 동일 결과 재사용 | 실제 인접42쌍 입력 완전일치0 | 동일질의 가정 금지 |
| terrain negative backoff8/16 | family-disposition=FAMILY_LOCAL_GATE_FAILED_NOT_ADOPTED | 더 큰 상수 튜닝 금지 |
| Solve 뒤0변위 조기거절 | 이미 검토됨, 기존8명 낙관적 제거량0.8885%/1.0770% 전체solve | 새 후보로 반복하지 않음. CPU 절감률 아님 |
| 발ray 하단 축소 | 실제 엔진 lift차 최대약0.305mm, 기존 strict gate 기각 | 허용오차를 조용히 바꾸지 않음 |
| RaycastRV 대체 | mask/shape/normal·hit 계약 동등성 및 비용 미입증 | drop-in 구현 금지 |

이 표는 전 프로젝트의 완전한 미검증 후보 부재를 증명하지 않는다. 이번 제한된 검토에서 생산 코드에 바로 넣을 만한 새 고가치 안전 후보를 확보하지 못했다는 결론이다. 따라서 억지로 미세 변경을 만들거나 비용 개선을 주장하지 않는다.

## 5. 검증 수준
현행 런타임 마스크 구성 진단 성공. 새 기능·시각·성능 개선 또는8/64 회귀 통과가 아니다. 이전 gate의 기각을 유지하며 전체 목표는 미완료다.

## 6. 변경 사항
무개체 미션·핀 고정 계획·runtime TSV·분석/보고서만 추가. 기존 게임 코드·10개 PBO·서버/클라이언트 바이너리·모델·원시 실험 불변. 새 PBO가 없으므로 신규 서명 대상 없음. 역할LOD 후보를 활성화하지 않았다.

## 7. 원시 증거
ANIM-physics-mask-contract1. mask-values.tsv·plan.json·mission/init.c·server-terminal.json·script 로그와 기존 현행서버 cost64-three2 REPORT/family-disposition/conformance-summary 및0변위검토를 입력 해시로 연결한다. 저장 전 전체 입력 해시·JSON 및 시험 프로세스 부재를 확인한다. 공개에는 로컬 경로·PID·키·게임 자산을 제외한다. 공식 API 참고:https://github.com/BohemiaInteractive/DayZ-Script-Diff/blob/main/scripts/3_game/global/dayzphysics.c . 원격 선언은 설치 바이너리의 실행 동등성 근거가 아니며 실제 마스크 숫자는 이번 현행 서버 실행에서 얻었다.

## 8. 잔여 문제와 다음 단일 산출물
새 전용 실행을 늘리지 않고 기존64 밀집 비용 기록의 시간 구간별 기여를 재검토한다. 다음 산출물은 같은 실행에서 실제 높은 비용 구간과 살아있는 개체 수·상태·Drive/Displace 횟수·실제 프로세스CPU가 연결 가능한지 표시한 구간표다. 기존 평균·inclusive 표본·호출량을 더하거나 순수 비용으로 바꾸지 않는다. 기존 데이터가 이미 그 표를 제공하면 재생성하지 않고 해당 근거를 연결한다. 연결이 불가능하면 어떤 필드/동시 구간이 부족한지 밝히며 무조건 새 계측기를 만들지 않는다.
발ray 수치 허용범위를 바꾸는 별도 제안은 아직 사용자 응답을 기다리는 선택사항이며 현재 gate는 그대로다. 그 답변을 모든 독립 분석의 차단 조건으로 취급하지 않는다. 기능 복구나 등반 자리 제한 개발로 확대하지 않는다.
