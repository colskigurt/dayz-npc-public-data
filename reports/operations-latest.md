# AntifreeZe와 Expansion AI의 대량 호드 적용 분석

작성: 2026-11-04T19:13:22.824017+09:00

## 1. 작업 목표

AntifreeZe와 Expansion AI의 실제 소스를 확보해 DayZ 대량 호드에 적용할 책임·호출 위치·비용·실패 조건을 판정한다. 이 세션은 대량 호드 설계 검토이며 애니메이션 시간 문제는 별도 세션에서 관리한다. 이번 산출물은 소스 분석과 첫 구현 명세다.

핵심 결론: Expansion의 중앙 AI 판단 스케줄러가 우선 참조 대상이다. AntifreeZe에서는 사건 기반 깨우기·재검사 분산을 참고하되 native AI 동결·전체 CommandHandler 생략을 그대로 이식하지 않는다. 두 프로젝트 모두 수천 개체의 저비용 렌더링이나 공유 Flow Field를 제공하는 근거는 이번 분석에서 확보되지 않았다.

## 2. 실제 수행

2026-11-04에 두 공개 저장소를 research/horde-ai-source-review-20261104 아래 격리 clone했다. AntifreeZe는 master의 2a805e19546a3504f2fa3dfde90235e20a1ce7a0, Expansion은 experimental의 14fd692bd7eb46df05853ea31ee3d481239a826c를 고정했다. Expansion은 AI·Navigation 및 선정 Core 경로만 sparse checkout한 참고본이다. 실행·컴파일 가능한 전체 Expansion 설치를 확보했다는 뜻이 아니다.

AntifreeZe의 CommandHandler 전체와 동결·깨우기·밀집·확률·설정/사망/재로딩 흐름을 읽었다. Expansion은 DayZGame→ExpansionGame/World 갱신, ExpansionWorld의 중앙 스케줄러, eAIBase의 판단/명령 분리, 순찰 한도·분산 생성·제거, 목표 후보 순회, 경로 재계산 조건을 읽었다. eAIBase 전체를 완독하거나 모든 무기/차량/FSM 경로를 감사한 범위는 아니다.

로컬 NorthFrontierHorde의 Coordinator/Spatial/Perception/Navigation/Zone 설정과 DGOriginalSample의 GroundDirector/GroundAgent/Crowd/Traversal/Perception 소스를 대조했다. NH 감염자 경로와 DGB Animal 기반 커스텀 경로는 별개다. 현재 실행 PBO·network override 전체와 동일함을 입증한 비교는 아니다. 기존 working tree 수정은 보존했다.

## 3. 측정 결과

새 게임 실행·컴파일·PBO 변경·성능 측정은 각각 0회다. 아래 상수는 upstream 설정/코드이며 우리 후보의 채택값이나 측정 결과가 아니다. 다운로드 파일 수와 SHA-256, Git commit·선정 행 범위는 source-manifest.json과 analysis.json에 저장한다.

| 구분 | 코드에서 확인한 값·구조 | 해석 경계 |
|---|---|---|
| AF 높이 판단 | 기본 수평 6m 내, 목표가 1.25m 위인 상태가 0.6초 지속하면 동결 | 계단·경사로·등반 링크의 도달성 검사 없음 |
| AF 추격 token | 거리별 확률 clamp 0.04..0.55, TTL 1.2초+jitter | 전역 token pool·활성 수 hard cap·최대 서비스 대기 보장 없음 |
| AF 동결 aging | 기본 0.75초 누적 후 super에 최대 0.06초 전달하고 누적값 0으로 초기화 | 해당 호출에 전달 안 된 시간은 다음 누적에 보존되지 않음. native 기억이 정확히 몇 배 느려지는지는 미측정 |
| AF 밀집 | 개체마다 새 배열+SceneGetEntitiesInBox, 기본 반경 2m·이웃 6명 | broadphase 공유 없음. 조회 호출 자체 비용은 미측정 |
| AF wake | 피격·접촉 뒤 기본 3초 grace, 0.3초 wake cooldown | 집단 접촉 시 깨우기 폭주 및 가까운 거리 상태 반영 지연은 실제 시험 필요 |
| Expansion 판단 | 기본 AI_UPDATE_INTERVAL 0.05초; FTO=2일 때 가시 거리 내 player 목록이 비면 0.25초 | 엔진 simulationPrecision setter나 애니메이션 시계를 바꾸는 기능과 구분 |
| Expansion batch | 기본 최대 20개, 목표 예산 min(frame budget 30%, 5ms), 실제 비용을 호출 뒤 확인 | FTO>0 분기. 낮은 서버 FPS에서 산출식이 달라지고 한 단일 호출이 예산을 넘을 수 있어 5ms hard cap 아님 |
| Expansion patrol | Global/Category MaxPatrols, 접속자 구간별 선택, 접속/종료 변경 10초 debounce | CPU 자동 계측 기반 한도가 아니라 설정 기반 순찰 수 한도. 개체 수 한도와 다름 |
| Expansion 생성 | leader 생성 후 나머지를 CallQueue로 deferred 생성 | 명시 ms/프레임 생성 예산이 아님. 실제 프레임 분산은 미측정 |

## 4. 원인 분석

### AntifreeZe에서 참고할 부분과 이식 금지 경계

CommandHandler는 disabled/dead/finisher→grace→frozen→non-MOVE 통과→높이 검사→far CHASE 밀집/token→super 순서다. frozen 처리가 non-MOVE guard보다 먼저이므로 주석만으로 모든 공격·넘기 명령이 항상 먼저 통과한다고 해석하면 안 된다. 타이밍 조건상 해당 상태가 실제 도달하는지는 미실행이다.

FreezeFor는 SetKeepInIdle(true), WakeFor는 false를 쓴다. NHMember도 Attach/Release에서 같은 AI 상태를 소유한다. 독립 모드 두 개가 이 bool을 서로 바꾸면 스크립트 명령 소유권·원래 AI 복귀를 깨뜨릴 가능성이 있다. DGB 경로는 Animal_DGNativeProbe와 자체 Drive/Traversal을 사용하므로 ZombieBase 패치만 붙여서는 적용되지 않는다.

높이 규칙은 '공격할 수 없다'와 '걸어가거나 올라갈 수 없다'를 혼동할 수 있다. 우리 호드는 실제 링크/지지/층 ID와 최근 경로 실패·진행량으로 재계획 간격만 늦춰야 한다. CLIMB/MOTION·FALL·착지·공격·피격·사망의 진행을 동결 대상으로 삼지 않는다.

확률 token은 동시 활성 수를 보장하지 않는다. 확률 판정이 모두 성공할 수도 있고, 연속 실패하면 개체가 오래 기다릴 수 있다. 우리 구현에는 전역 예산·호드별 공정성·최대 판단 지연과 마지막 처리 시각이 필요하다. 대기 중인 앞개체를 얼려 출구를 막으면 후미도 막힐 수 있으므로 통로 점유·목표 접근과 별개로 정책을 평가한다.

AF의 hot disable 분기는 super 호출만 하고 기존 frozen flag/KeepInIdle을 해제하지 않는다. 옵션 변경을 롤백으로 사용할 때는 동결을 소유한 모든 개체의 명시적 해제·타이머 정리가 필요하다. 사망 시 1.5초 후 DisableSimulation과 조건부 시체 제거도 있으므로 README의 '스폰/전투 미변경'만 보고 설치 범위를 판단하면 안 된다. 현재 시체 충돌·전리품·지지 계약에 독립 검증이 필요하다.

Config에는 기본 비활성화되지 않은 telemetry 예약 경로, MissionServer에는 기본 OFF인 chat reload가 있다. 이번 clone은 실행하지 않았다. 우리 자체 설계에 이런 부가 기능을 가져올 필요는 없다.

### Expansion에서 우선 참고할 실제 구조

DayZGame.OnUpdate(server)→ExpansionGame의 eAI_OnUpdate 인터페이스→ExpansionWorld 구현→alive 연결리스트 cursor→기한이 된 eAIBase 판단 순서다. FTO>0에서는 eAIBase.CommandHandler의 직접 판단/FSM 갱신이 빠지고 중앙 경로가 실행한다. FTO=0일 때는 직접 경로다. 별도 AI thread 호출과 Sleep 루프는 주석 처리돼 있어 현재 코드가 AI 작업을 별도 native worker로 병렬 실행한다는 근거가 아니다.

중앙 경로는 개체별 lastUpdateTime으로 판단 dt를 계산하고, 비용 이동평균으로 batch 크기를 정한 뒤 실제 비용/개수에 도달하면 멈춘다. 연결리스트 제거 이벤트가 현재 cursor를 다음 노드로 이동시킨다. 예산은 호출 후 확인하므로 비싼 FindPath 하나를 중간에 선점하지 못한다. 평균 비용만 보고 batch를 정하면 긴 꼬리 비용을 예상하지 못하고, due가 아닌 개체도 루프에서 훑을 수 있다.

실제 frame CommandHandler는 계속 무기·명령 종료·상태 전이를 관리한다. 따라서 적용할 핵심은 '판단을 나누되 실행을 유지한다'다. 우리의 PrePhys/Drive dt를 마지막 판단 이후 경과시간으로 대체하면 물리 이동·루트모션이 커지는 부작용이 생긴다. 기억 만료는 monotonic wall time, 판단 age는 별도 필드, 물리 dt는 엔진 입력으로 분리해야 한다.

Expansion UpdateTargets는 30m 상자 검색을 캐시하고 player 연결리스트를 호출당 하나씩 순회한다. 후보 index와 일부 break 조건도 있다. 모든 후보에 대한 고정 ms 예산은 아니다. 우리 호드는 약탈·장비·엄폐 후보 대부분이 불필요하며 플레이어 목록/사건을 director에서 공유할 수 있다. 공유 후보는 각 개체의 실제 시야 확인을 대체하지 않는다.

LoadBalancing은 순찰 수 기준이다. 5명 순찰과 100명 호드를 같은 1로 세면 대량 호드에는 부족하다. 활성 호드 수·실체 개체 수·판단 비용·예산 초과를 함께 계수해야 한다. ClearAI는 위협이 남은 개체의 제거를 지연할 수 있고 저장 그룹도 지원하지만, 멀리 있는 호드의 위치·경로·사망을 계속 계산하는 가상 인구 시스템은 이번 경로에서 입증되지 않았다.

PathHandler는 재계산 표시·재시도 시간·현재 경로·속도에 의한 overshoot 조건을 분리한다. 인스턴스가 eAIBase를 소유하는 개별 경로이며 공유 Flow Field가 아니다. PlayerBase 이동/사다리/무기/문 처리와 결합돼 DGBGroundAgent로 직접 교체할 수 없다.

### 우리 코드와 연결할 구체적 위치

| 후보 | NH 감염자 경로 | DGB 커스텀 경로 | 실제 새 작업 |
|---|---|---|---|
| 중앙 판단 budget | NHCoordinator.Tick의 전체 member 루프와 PathBudget | DGBNativeGroundDirector.Update의 감각·intent·pursuit | 자체 HordeDecisionScheduler, job별 cost/기한/대기 cursor. 경로는 요청 수와 시간 모두 계수 |
| 단계적 감각 처리 | NHPerception.Sense, 공유 GatherPlayers | GroundDirector.UpdatePerception, Agent.SenseAt | 기존 SenseSeconds 재사용. 만료 검사와 후보 검색/LOS를 분리하고 phase를 분산 |
| wake 우선권 | 피격·접촉·목표 변경 이벤트→판단 요청 | Combat/Traversal 사건→director urgent queue | 중복 사건 합치기, hit/death 즉시 처리, 우선 판단의 예산 초과 기록 |
| 공통 공간 자료 | NHSpatial의 6m 셀 | DGBNativeCrowd.Cells와 접촉 candidate | AF 개체별 Scene query 추가 대신 기존 snapshot 재사용. stale snapshot은 안전 검사 생략 근거 아님 |
| 수명/활성 예산 | NHZoneManager의 single Active·GlobalActiveLimit=64 | GroundDirector.Spawn의 고정 fixture cohort | multi-horde registry와 전역/호드별 실체 수, spawn cost budget, 생성 예약·실패 환불·세대 ID |
| 도달 불가 backoff | NHNavigation 유지 경로/PathRetrySeconds | DGBGroundAgent.PlanAt/RetainedRouteRefreshes | 이미 있는 backoff를 중복 구현하지 않고 최근 실패·목표/장애물 revision에 연결 |

NHCoordinator.Cursor는 순서를 회전하지만 매 Tick 전체 Members를 방문한다. DGB는 player 수집 0.2초·SenseSeconds 기본 0.5초와 재계획 0.7초 guard가 이미 있어 '전부 매 프레임 LOS/FindPath를 실행한다'고 볼 수 없다. DGB 보고 전달은 보고별 전체 수신자를 훑는 경로가 있고, dense contact는 공간 분할 후에도 이웃이 많을 수 있다. 이 부분은 새 기능을 붙이기 전에 별도 계측할 후보이며 비용 우열은 미측정이다.

### 첫 구현 명세와 성공/실패 분기

첫 단일 산출물은 gameplay를 바꾸지 않는 scheduler shadow 계측이다. 기존 판단 요청을 job 종류·개체/세대 ID·요청 시각·마지막 처리 시각·호드 ID·긴급 이유로 기록하고, budget이 있다면 어떤 요청이 얼마나 밀리는지만 계산한다. 관찰 자체 비용과 기존 실행 순서/호출 수 불변을 함께 확인한다. 소스 분석만으로 이 계측을 실행한 것으로 기록하지 않는다.

후보 scheduler는 긴급 사건/기한 임박/일반 판단 queue를 순서대로 서비스하고 비용을 호출 전후 계수한다. 단일 native 작업은 선점할 수 없으므로 '예산 소진 후 새 작업 시작 금지'와 '단일 작업 초과'를 구분한다. 결정 업데이트 종료 후 새로 생긴 개체는 다음 epoch에서 편입하고, 삭제/사망한 generation은 queue에서 무효화한다. 단순 스크립트 의사코드 단계이며 컴파일된 모듈은 없다.

shadow의 가장 오래된 판단 지연·처리량이 허용 조건을 만족하고 오버헤드가 작은 경우에만 감각/경로 중 한 종류의 실제 분산 후보를 만든다. 각 job의 허용 지연은 전투 접근 속도·감각 유지/만료 규칙에 맞춰 정해야 하며 Expansion의 50/250ms를 그대로 채택하지 않는다. FIFO만 쓰면 긴급 사건이 밀리고 우선권만 쓰면 후미가 굶으므로 deadline/aging과 호드별 할당을 함께 검증한다.

시험 장면은 야외 추적, 실제 계단/등반으로 도달 가능한 고지대, 진짜 도달 불가 지붕, 좁은 문 밀집, 피해/접촉 wake, 여러 플레이어, 생성 중 취소/사망/재접속이다. 기준과 후보는 같은 개체 수·목표·장면·로그 수준을 사용한다. 서버 frame dt·AI 비용·개체 판단 age·경로 요청 수·최장 정체·전투 반응·클라이언트 Present를 독립 측정한다. 최초 작은 국소 비교 뒤 유망한 최종 후보에 기존 검증 정책의 반복을 적용한다.

성공: 서버 CPU/프레임 꼬리 비용이 줄고 판단 age·도달·피격·사망·접촉·지지의 기존 계약이 유지된다. 실패: 절약보다 queue/스캔 비용이 크면 스케줄러 범위를 줄인다. 비용이 physics/render에 있으면 판단 분산 확장을 보류하고 표현 실험으로 돌아간다. 중단: 정상 등반 동결, 필수 물리 갱신 누락, 공격/사망 지연, 시간 손실, generation 오염, 한계 초과 대기가 나오면 후보를 OFF로 돌린다. OFF 전환은 자체 상태/예약을 회수해야 한다.

필요 자원은 기존 DayZ 서버/클라이언트, 엔진 스크립트 빌드/서명 도구와 현재 계측 도구다. 새 유료 AI 구매·별도 게임·native DLL은 이 판단 scheduler의 선행 조건이 아니다. 이 구조만으로 GPU 군중 렌더링/대역폭/대량 bone pose 비용이 해결되지는 않는다.

### 코드 이용 조건

AntifreeZe 소스는 GPL-3.0-or-later를 표시한다. 코드를 직접 포함하는 배포 후보는 표시·소스 제공 등 적용 조건과 다른 코드/자산의 결합 조건을 확인해야 한다. 별도 연구 clone과 배포물 포함은 구분한다.

Expansion eAIBase 등 선정 파일에는 CC BY-NC-ND 4.0 표시가 있다. 이를 자유 수정·재배포 가능한 MIT 소스로 취급하지 않는다. 직접 수정본을 우리 배포물에 넣는 경로는 허가 조건 확인이 필요하며, 우선 정책/책임을 설명하고 우리 요구사항에 맞춘 별도 구현을 설계한다. ND 공식 안내: https://creativecommons.org/licenses/by-nc-nd/4.0/ . 파일마다 표시가 같다고 가정하지 않는다. 이번에는 upstream 코드/자산을 공개 보고서나 배포 PBO에 복사하지 않는다.

## 5. 검증 수준

소스 확보·고정 commit·선정 함수/호출 문맥 대조·파일 안정성/해시는 정적 진단 범위다. 실제 효과·Enforce 컴파일·8/64 기능·천 개체 표시·멀티플레이·성능·시각 품질·최종 수용은 미실행이다. AF의 제어 흐름 위험은 코드에서 도출한 가설이며 게임에서 재현한 버그로 보고하지 않는다. 검토된 자체 코드 경로가 실행 PBO 전체와 같은지도 이번에는 미검증이다.

## 6. 변경 사항

추가: 격리 upstream clone, provenance/해시·선정 근거·호출 흐름, 분석 보고서·TXT·검토 패킷. 기존 Source·PBO·프로필·서버·게임·현재 애니메이션 실험은 변경하지 않는다. 새 PBO가 없어 새 서명/검증은 해당 없음이다. 기존 배포본 서명을 이번 검토에서 검증했다고 보고하지 않는다.

## 7. 원시 증거

source-manifest.json은 clone별 commit·remote·checkout 파일 크기/해시를, analysis.json은 선정 함수·행 범위·고정 upstream URL과 비교한 자체 코드 해시를 보존한다. source-evidence.private.txt에는 로컬 검토용 선정 함수 원문과 호출 문맥을 담고 범위를 명시한다. 공개 evidence는 upstream 고정 링크·선정 이유·분석과 자체 코드의 제한 발췌만 제공한다. 전체 clone·게임 자산·키·프로세스 인자·로컬 절대 경로는 공개하지 않는다.

대표 원문: AntifreeZe scripts/4_World/Antifreeze/ZombieBase.c와 scripts/3_Game/Antifreeze/Config.c; Expansion AI의 Classes/ExpansionWorld.c, Entities/AI/eAIBase.c, Classes/Patrols/eAIDynamicPatrol.c, Classes/PathFinding/ExpansionPathHandler.c. 공개 링크는 evidence 패킷에 파일별 행 범위로 고정한다.

## 8. 잔여 문제

우선순위는 자체 판단 scheduler shadow 설계→한 job의 분산 실행 검증→다중 호드/실체 예산이다. AF 일괄 설치, 높이 차 동결, 전체 명령/물리 프레임 skipping, upstream 상수의 무근거 채택은 보류한다. 사용자 요청은 이번 구체 분석까지이며 신규 게임 시험이나 후보 배포를 수행한 것으로 기록하지 않는다.

수천 개체의 표현 비용·네트워크 interest 관리·다층 공유 경로장·상세/간소화 전환·원거리 전투는 별도 미해결이다. 두 저장소를 확보한 것은 이 문제들이 해결됐다는 근거가 아니다. 별도 세션의 애니메이션 작업과 경쟁하는 실험을 자동 시작하지 않는다.
