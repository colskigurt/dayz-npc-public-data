# 실제 비용 절감 후보 재선정 — 구현 전 경계 검토

## 1. 작업 목표
기존 검증을 보존하며64개체 전체CPU를 줄일 수정 지점이 남아 있는지 조사한다. 단순 감속·검사 삭제·이미 실패한 후보의 재실행을 피한다. 이번은 소스/기존 보고서 검토이며 새 게임 실행이나 구현이 아니다.

## 2. 실제 수행
현재 DGBNativeGround.SolveContactsInternal/DriveGround, DGBNativeCrowd, DGNativeProbe 및 실제5파트 생성 override NFPaletteFiveGameplay.Initialize를 확인했다. 기본 오프라인 Initialize의 ECE_LOCAL과 실제 네트워크 override의 ECE_CREATEPHYSICS를 구분했다. 소스 검색은 ignore를 해제한 보고서 범위를 사용했다. 검색으로 기록을 못 찾은 것을 미실행 확정으로 보지 않는다.
기존 회전 산술 REPORT, 서버 follower LOD의 클라이언트 포즈 실패 REPORT, 로컬 P3D 표현 연구 report-body, 최근 접촉 차단/접근 감속 보고서와 대조했다.

## 3. 측정 결과
새 CPU/Present/본 포즈/화면 품질 측정0, 새 독립 게임 실행0. 다음 수치는 기존 증거의 재인용이다.

| 후보 | 기존 확인 | 이번 결정 / 재개에 필요한 차이 |
|---|---|---|
| SupportSweepFast | 현재 엔진64쌍에서 Ray 감소에도CPU 개선 미수용 | 같은 sweep 대체를 반복하지 않음. 질의 수 감소 자체는 채택 근거 아님 |
| Scratch/결과 재사용 | 작은 국소 효과 또는 입력 불일치 | 정확히 같은 입력·수명·전체CPU 근거 없이는 재개하지 않음 |
| Drive→Displace 결합 | 기존42쌍 완전 입력 일치0 | 중간 위치/heading/history가 달라 그대로 합치지 않음 |
| 회전 sin/cos 재사용 |65,536좌표 숫자동등, 국소 중앙 약41.79% 감소 | 전체 효과 미측정이며 조건부 규모 환산 약6.95ms/게임초. 주력 병목 해결책으로 승격하지 않음 |
|4표시파트 | 전체CPU 한 쌍 효과, 다음 쌍5% 문턱 미달 | 유리한 재실행으로 채택하지 않음. 조건/기전이 바뀌어야 재개 |
| 서버 follower LOD1 | 클라이언트 RightHand 최대0.26055m 차이 | 포즈 보존 실패. 네트워크 독립성 가정 폐기 |
| follower solid 변경 | 서버 변화 작고 클라이언트 악화 관찰 | 현재 방식 미수용. native 물리 비용 전체 제거로 확대 해석 금지 |
| 교차표시파트 선택 차단 |64체 CPU 관찰 증가, client cadence 불일치 | 미수용·인과 미확정. 쌍별 설정48,384개는1000체 확장 해법 아님 |
| 후열 감속 | 안전 구간 근사 호출 비중 약0.9~1.2%, speed만 낮춰도 solver 지속 | 구현 보류. 실제 입구 보호·native root 제어와 총CPU 근거 필요 |
| VAT/공유 pose | 검토한 공개 모딩 경로로 지원 인터페이스 미확보 | 다른 엔진의 기술을 곧바로 적용 가능한 것으로 주장하지 않음 |

## 4. 원인 분석
현재5파트 구현은64논리체에 표시320체+원본 피해 수신64체를 서버에서 생성한다. 리더만 GroundBody.Drive를 호출하고 지지·관절 검사에 쓰는4개 본과 EntityPosition도 리더에서 읽는다. 반면 보조 표시파트도 DGNativeCommand와 동기화·퇴역 수명에 참여한다. 이 코드 의존성은 보조파트 제거의 기능 안전성을 증명하지 않는다.

남은 구조적 연구 후보는 **서버의 보조 표시파트4개를 클라이언트 전용으로 분리**하는 방식이다. 서버에는 리더+기존 피해 수신체를 유지하고, 클라이언트는 원래5파트 전체 모델을 유지한다는 후보 계약이다. 이는 저폴리화·프레임 생략·지지 판정 삭제와 다르다. 모든 NPC를 화면에서 유지한다.

절감 원리(미입증): 서버에서4×64개의 보조 캐릭터 호스트와 관련 갱신/복제 작업을 만들지 않는다.384→128은 구성 개수 산술일 뿐CPU66.7% 절감 예측이 아니다. 리더/수신체의 애니메이션과 모든 기존 물리 질의는 남는다. 실제 총비용에서 보조 호스트의 기여가 얼마나 되는지는 분리 미확인이다.

중요한 막힘: 현재 잘 작동하는 클라이언트 애니메이션은 네트워크 모드 경로이고 서버 LOD 변경에도 클라이언트 포즈가 달라졌다. 로컬 생성 파트가 같은 그래프 시계·위상·블렌드·14동작 전환을 보존한다는 근거는 없다. NativeSeconds와 상태 번호만 같아도 동일 포즈가 되지 않을 수 있다. 기존 duration 인자를 phase/dt로 사용하는 방법은 기각된 해석이므로 사용하지 않는다. 로컬 P3D user-source 시험도 현재 원본 skeletal 모델의 재생을 증명하지 않는다.

### 확인한 역할별 접근 계약

| 항목 | 현재 확보한 접근 | 분리 후보의 부족한 계약 |
|---|---|---|
| 동작 선택 | DGNativeCommand.PreAnimUpdate에서 RequestedState/TraversalReleased/LandingConfirmed 설정 | 새 로컬 파트의 상태 전달·새 접속자 초기화·전환 세대/순서 보장 없음 |
| 시간 관측 | NativeSeconds는 스크립트 Age+=pDt | 그래프 phase/블렌드 상태를 읽고 같은 시각으로 설정하는 공개 경로 미확보 |
| 인스턴스 선택 | DayZCreature.SetAnimationInstanceByName(name,uuid,duration), GetCurrentAnimationInstanceUUID 선언 | UUID/duration을 phase나 누적dt로 사용하는 것은 정당화되지 않음 |
| 정밀도 | UpdateSimulationPrecision(simLOD) 선언 및 기존 실제 시험 | 서버 변경이 클라이언트 포즈와 독립적이지 않았음 |
| 위치 표시 | 승인된 논리 위치의 Presentation history, 엔티티 transform | 위치를 맞춰도 내부 포즈 시계/전환 상태 일치를 보장하지 않음 |
| 포즈 관측 | GetBonePositionMS 및 기존 공통 본 진단 | 관측 API가 다른 엔티티의 pose를 주입/공유하는 API는 아님 |

위 선언은 보존된 DayZ Script Diff의 scripts/3_game/entities/dayzanimal.c를 확인했다. 공개 소스 스냅샷이 현재 설치 네이티브 구현 전체를 증명하지는 않는다. 전역 API 부재 판정 대신 이 후보를 구현할 필수 제어점을 아직 확보하지 못했다는 판정이다.

## 5. 검증 수준
리더 중심 gameplay 의존성과5파트 네트워크 생성 구조: 소스 확인. 서버/클라이언트 분리 후보: 설계 가능성 검토만 완료. 로컬 원본 파트의 동일 네트워크 애니메이션 시계·일치·화면 품질·8/64 기능·CPU/Present: 미검증. 전체 엔진 API에 이런 기능이 절대로 없다는 판정도 하지 않는다. 조사 범위에서 지원 경로를 확보하지 못했다.

## 6. 변경 사항
이 문서와 근거 해시만 추가한다. NPC 게임 코드·PBO·모델·배포본·기존 실험/보고서 변경0. 새로운 서명 대상 없음. 후보 구현이나64 회귀를 시작하지 않았다.

## 7. 원시 증거
현재 소스: build-candidate1/source/nf_palette_five_gameplay/Scripts/4_World/NFPaletteFiveGameplay.c, DGNativeProbe.c, DGBNativeGround.c, DGBNativeCrowd.c.
기존 연구: solve-rotation1/REPORT.md; host-physics-readback1/server-follower-lod-probe1/client-role-probe1/REPORT.md; horde-render-paths-20261104/report-body.md; selective-peer-block1/cost64-screen1/REPORT.md 및 APPROACH_SLOWDOWN_REVIEW.md. 동일 실행 재분석을 새 반복으로 세지 않는다. 파일별 SHA256은 이 문서의 evidence JSON에 보존한다.

## 8. 잔여 문제
현재 조사 범위에서 **즉시 적용 가능한, 큰 전체CPU 절감이 입증된 국소 후보는 없다**. 이것은 최적화 불가능의 증명이 아니라 확보된 근거의 한계다. 작은 합성 테스트의 좋은 비율로 이를 대신하지 않는다.
후보 구현 전에 필요한 최소 산출물은 기존 애니메이션 계층의 로컬/네트워크 역할별 clock·상태·phase 접근 계약표다. 이 표에서 지원 경로가 확인될 때만 단일 NPC의 원본5파트 조립체로 동일시점 본 일치 및 실제 화면을 비교한다. 실패하면8→64나 새 동기화 시스템 개발로 확대하지 않는다. 성공해도 spawn/despawn, 새 접속자·late join, 순간이동, 피격/사망/시체,14클립 전환 및8→64 회귀와 동일 foreground/render 조건의 반복 CPU·Present 비교가 필요하다. 클라이언트 연산·복제/상태전달 비용이 늘 수 있으므로 반드시 총비용으로 판정한다.
이 후보는 단순 국소 수정보다 범위와 위험이 크다. 기존64체 근접 목표를 원거리LOD로 바꿔 달성했다고 보고하지 않는다. 지원 시계/phase 경로가 확보되지 않으면 구조 변경을 구현하지 않고, 엔진 제약과 필요한 개발 범위에 대한 사용자 판단을 요청한다.

