# 외부 호드 코드 분석 — DayZ 적용 가능 범위

분석 시각: 2026-10-30T18:50:11.820358+00:00

## 1. 작업 목표

GPUSkinning과 crowd-simulation의 실제 소스에서 현재 DayZ NPC에 참고할 설계, 직접 이식할 수 없는 엔진 의존성, 아직 검증하지 못한 부분을 구분한다. 분석 ID: external-horde-source-review-20261031-034547. 기존 NPC 코드·PBO·씬·실험 설정 변경과 클라이언트 실행은 제외한다.

## 2. 실제 수행

- GPUSkinning 고정 커밋 `419fdbce04e4dfc50afc6988040fae96e0ea7812`, crowd-simulation 고정 커밋 `19811373b651fe135eb16cf7a9ae7aed1ffaf67a`의 트리를 조회했다. 소스/설명/라이선스 등 선택된 텍스트 72파일, 319,299바이트만 수집했다. 모델·영상·UE Content 바이너리·PDF는 다운로드하거나 실행하지 않았다. 수집 파일 전체를 정독했다는 의미는 아니다.
- GPUSkinning의 Player/Resources/Sampler/Include/Util/once-per-frame 경로와 crowd의 CustomAvoidance/CustomNavigation/NavMeshMovement/MoveToTower/fragment/module 경로를 중심으로 읽었다.
- DayZ의 DGNativeCommand, DGBNativeGroundBody, DGBNativeCrowd, DGBGroundAgent 및 현 후보의 MotionContracts/ContactInputs/DriveAttribution 층을 대조했다. SDK dayzanimal.c의 그래프 변수/명령 및 물리 콜백 인터페이스를 확인했다. 현 후보의 전체 실행 설정과 모든 modded 호출 순서를 재검증한 것은 아니다.

## 3. 측정 결과

| 항목 | 이번 결과 |
|---|---|
| 외부 소스 무결성 | 72/72 파일 SHA-256 재계산 일치; 커밋/원문 URL 보존 |
| 로컬 비교 입력 | 21개 파일 해시 기록; 보고서 저장 시 재검사 |
| 후보 PBO | 기존 packed-source-proof의 PBO SHA와 일치 |
| 1/8/64개체 실행 | 이번 분석에서는 미실행 |
| p50/p95/p99, CPU/GPU 비용 | 미측정; 기존 실험 값을 이번 성과로 재사용하지 않음 |
| 실제 화면 부드러움 | 미관찰·미판정 |
| 회귀·최종 수용 | 미실행·미수용 |

## 4. 원인 분석

### GPUSkinning: 실제 확인

1. [Sampler](https://github.com/chengkehan/GPUSkinning/blob/419fdbce04e4dfc50afc6988040fae96e0ea7812/Assets/GPUSkinning/Scripts/GPUSkinningSampler.cs#L240)에서 클립별 fps로 샘플링하고, 본 행렬을 RGBAHalf 텍스처에 기록한다. 정점 UV 채널에 본 인덱스/가중치를 저장한다. 이는 정점 위치 전체를 저장하는 VAT와 구분되는 **본 행렬 텍스처 기반 재생**이다.
2. [Player](https://github.com/chengkehan/GPUSkinning/blob/419fdbce04e4dfc50afc6988040fae96e0ea7812/Assets/GPUSkinning/Scripts/GPUSkinningPlayer.cs#L515)에서 반복 동작은 공유 resource time + 선택적 개체별 위상 차이, 일회 동작은 개체 시간을 사용한다. 프레임 선택은 `int(time * fps) % int(length * fps)`이다. [Resources](https://github.com/chengkehan/GPUSkinning/blob/419fdbce04e4dfc50afc6988040fae96e0ea7812/Assets/GPUSkinning/Scripts/GPUSkinningPlayerResources.cs#L214)는 공유 시계를 프레임당 한 번 갱신한다. 이것은 Unity Time.frameCount에 의존하며 DayZ 그래프 시계를 제어하는 API가 아니다.
3. [셰이더](https://github.com/chengkehan/GPUSkinning/blob/419fdbce04e4dfc50afc6988040fae96e0ea7812/Assets/GPUSkinning/Resources/GPUSkinningInclude.cginc#L25)는 선택된 프레임의 본 행렬을 텍스처에서 읽고 정점을 변형한다. 클립 간 crossfade는 있지만 확인한 경로에는 현재/다음 프레임 사이의 시간 보간이 없다. Util은 Point 필터를 사용한다. **GPU 재생이라는 이유로 낮은 샘플률의 시각적 계단 현상을 해결한다고 볼 수 없다.**
4. [UpdateMaterial](https://github.com/chengkehan/GPUSkinning/blob/419fdbce04e4dfc50afc6988040fae96e0ea7812/Assets/GPUSkinning/Scripts/GPUSkinningPlayer.cs#L412)은 클립/프레임 인덱스가 같으면 조기 반환한다. 프레임이 건너뛰면 이벤트는 현재 프레임과 일치하는 것만 검사하고, root delta도 선택된 프레임을 처리한다. 프레임 구간을 모두 순회하는 보완 경로는 해당 함수에 없다. 이벤트 누락·root 이동량 손실·같은 프레임에서 crossfade 가중치 갱신 지연은 검토할 정적 위험이며 실제 재현 시험은 하지 않았다.
5. [UpdateJoints](https://github.com/chengkehan/GPUSkinning/blob/419fdbce04e4dfc50afc6988040fae96e0ea7812/Assets/GPUSkinning/Scripts/GPUSkinningPlayer.cs#L583)는 노출 joint를 CPU에서 갱신하며 애니메이션 혼합 시 joint 갱신 TODO가 남아 있다. GPU 표시 포즈와 CPU 공격/발 지지 포즈를 일치시키는 문제가 자동 해결되지 않는다.

### crowd-simulation: 실제 확인

1. [CustomNavigationProcessor](https://github.com/gianmarcopicarella/crowd-simulation/blob/19811373b651fe135eb16cf7a9ae7aed1ffaf67a/Source/MyProject2/CustomNavigationProcessor.cpp#L68)가 개체 XY 좌표 점군을 구성한다. [CustomAvoidanceProcessor](https://github.com/gianmarcopicarella/crowd-simulation/blob/19811373b651fe135eb16cf7a9ae7aed1ffaf67a/Source/MyProject2/CustomAvoidanceProcessor.cpp#L246)는 점군을 받아 2차원 nanoflann KD-tree를 구성하고, 이웃 목록 조회를 세 slice로 나눠 갱신한다. 이는 **이웃 검색 분산**이지 본 포즈를 3프레임마다 재생하는 설계가 아니다. 회피 force 계산은 별도 chunk 순회에서 수행한다.
2. [회피 연산](https://github.com/gianmarcopicarella/crowd-simulation/blob/19811373b651fe135eb16cf7a9ae7aed1ffaf67a/Source/MyProject2/CustomAvoidanceProcessor.cpp#L603)은 후보 목록에서 최대 6 collider를 처리하고, 상대 위치/속도와 미래 최접근 시각으로 분리·예측 회피 force를 계산한 뒤 가속도를 제한한다. `reserve(6)`은 검색 결과 제한이 아니다. 이웃 검색/목록 생성 비용과 6명에 대한 force 비용을 구분해야 한다. 밀집 상태에서 전체 비용이 개체당 상수라는 보장은 없다.
3. 상대 위치 Z를 0으로 두는 **평면 가정**이 코드에 명시돼 있다. DayZ에서는 Y가 높이이며 현재 Crowd는 높이 차이 필터를 갖고 있다. 평면 알고리즘을 그대로 복사하면 지붕 위아래 개체 사이에 허위 회피력이 생길 수 있다. 현재 등반 중 가장자리 슬라이드/착지 공간 검사를 제거할 근거가 아니다.
4. [NavMeshMovementProcessor](https://github.com/gianmarcopicarella/crowd-simulation/blob/19811373b651fe135eb16cf7a9ae7aed1ffaf67a/Source/MyProject2/NavMeshMovementProcessor.cpp#L45)는 개별 path result/point index를 보관하고 FindPathSync를 호출한다. 완료 시 myHasMoved로 skip하는 임시 코드와 고정 중심 주변 무작위 목표 선택이 있다. **공유 flow field나 움직이는 플레이어에 대한 완성형 추적 루프라고 설명할 수 없다.** README도 도시 광장으로 모이는 게임을 설명한다.
5. Mass fragment/chunk/processor와 NavigationSystem/nanoflann은 C++·Unreal 모듈에 의존한다. 반복 실행되는 점군 복사·KD-tree 생성 및 최초 slice 배정 이후 추가 스폰 처리 등은 별도 검토 대상이다. 저장소의 2만+ 개체 설명은 제작자 주장이고 우리 기능·환경의 성능 증거가 아니다. UE 바이너리 Content 안의 애니메이션/Blueprint 연결은 이번 텍스트 분석 범위 밖이다.

### DayZ 적용 판정

| 후보 | 판정 | 기존 구현과 조건 |
|---|---|---|
| 클립 샘플률/정수 프레임/클립 전환/표시 시각 분리 계측 | 현재 진단에 유용 | CPU 포즈와 화면 프레임의 대응을 먼저 관찰. GPUSkinning 구조가 DayZ 내부 구조라는 뜻은 아님 |
| 개체별 시작 위상·속도 차이 | 별도 시각 개선 후보 | 현재 끊김/0.5배속 원인 해결과 분리; root 이동·공격 이벤트 시각 일치 필요 |
| 가까운 개체만 조회하는 공간 색인 | 이미 유사 구조 존재 | 현재 Crowd는 1m XZ 격자·주변 9셀·높이 차이 검사로 pair를 수집. 새 KD-tree가 더 빠르다는 측정 없음 |
| 이웃 목록 조회 분산/저장 | Enforce 설계 후보 | 근접 충돌 자체는 유지. 피격·등반·착지·낙하·빠른 접근 시 즉시 무효화. 현재 접촉 계산을 통째로 건너뛰지 않음 |
| 회피용 이웃 수 제한 | 조건부 후보 | 실제 침투해소 pair를 잘라서는 안 됨. 회피와 실제 접촉을 분리한 비교 필요 |
| 예측 회피력·가속도 제한 | 개념 이식 가능 | 현재 ContactForce/ContactVelocity와 이중 제어·서로 다른 목표를 만들지 않는 별도 실험 필요 |
| GPUSkinning C#/셰이더 직접 이식 | 현 경로에서 직접 불가 | Unity MaterialPropertyBlock·instancing·사용자 vertex shader가 필요. 현재 AnimalBase/그래프 API에서 동등한 경로는 확보하지 못함 |
| Mass ECS/병렬 처리/nanoflann 직접 이식 | 직접 불가 | Unreal C++ 모듈을 Enforce가 실행하지 못함. 자료구조 원리를 다시 구현하는 것과 엔진 병렬 실행은 다름 |
| 동시 등반·밀려 낙하·재등반 | 해당 소스로 대체 불가 | 확인한 회피 경로는 평면 기반. 현재 발 지지·world sweep·상태/회복 계약을 보존해야 함 |

**엔진 제약의 확인 범위:** 설치 SDK에는 그래프 변수 바인딩, 인스턴스 선택, PreAnim/PrePhys/PostPhys 콜백, root translation 제어, 재질 파일 교체가 있다. 이들 인터페이스는 본 행렬 텍스처와 개체별 GPU 버퍼를 바인딩하는 Unity 경로와 다르다. 현재 공개 스크립트/프로젝트에서 동등 경로를 확보하지 못했다는 판정이며 엔진 내부에 그런 기능이 절대 존재하지 않는다는 증명은 아니다. native 엔진 변경의 가능성·비용은 이번에 조사하지 않았다.

**라이선스:** GPUSkinning 루트 MIT 확인. crowd-simulation 트리에 루트 LICENSE를 확인하지 못했고 Epic 저작권 표기가 있는 파일도 있다. 공개 열람 가능과 코드 재배포 허가는 구분한다. 이번에는 원리를 분석했으며 crowd 코드를 모드에 복사하거나 공개 미러하지 않았다.

**이번에 확정하지 않은 것:** DayZ 그래프의 시간 손실 원인, CPU 포즈가 실제 표시되는 시각, 현재 끊김이 샘플률/그래프 cadence/렌더 전달 중 어디서 발생하는지. 외부 코드 열람으로 이 중 하나를 원인으로 확정하지 않는다.

## 5. 검증 수준

- 고정 소스·호출 관계 확인: 정적 진단 성공(열람한 경로에 한함).
- DayZ 적용/컴파일/회귀/성능/화면 품질: 미실행. 반복 검증 완료·최종 수용 아님.
- PBO 해시는 기존 증거와 대조했다. 이번에 새 빌드·서명·서명 검증을 실행하지 않았다.

## 6. 변경 사항

NPC 소스·PBO·씬·실험 플래그를 변경하지 않았다. 이 분석의 소스 스냅샷/manifest/선정 발췌/보고서 및 공개 파생 요약만 생성한다. 기존 실행 ID·측정값·실험 시각은 갱신하지 않는다. 외부 소스 스냅샷은 약 0.32MB이며 전체 저장소를 clone하지 않았다.

## 7. 원시 증거

- external-horde-source-review-20261031-034547/source-manifest.json: 원문 URL·고정 commit·파일별 SHA·크기.
- source-review_evidence.txt: 시간 선택/셰이더/이웃 검색/경로 루프의 선정 발췌와 행번호. 전체 코드 감사가 아님.
- local-inputs.json: 21개 로컬 입력 해시; 로컬 경로 포함 파일은 공개하지 않음.
- plan.json 및 두 repository-tree.json: 생성 전 범위와 트리 목록. 이번 source manifest는 GitHub가 제공한 byte snapshot의 해시이며 과거 파일 동일성 인증은 아님.
- 코드·영상 실행 ID 없음. 화면 녹화 없음. 기존 실험 원본을 수정하지 않았다.

## 8. 잔여 문제

1. 우선 현재 1개체 고정 카메라 시험에서 같은 루프의 실제 화면 위상·반복주기를 CPU 포즈/콜백/dt와 맞춰 비교한다. 성공 기준은 실제 화면에서 반복/정지 프레임을 구별하고 CPU와 시간 대응을 설명하는 것. 화면 관찰을 확보하지 못하면 원인 판정을 중단하고 촬영/도구 조건부터 보완한다. 새 보간을 넣는 단계가 아니다.
2. 64개체 이웃 검색·접촉·world sweep 비용 귀속은 별도 실험으로 관리한다. 비용이 입증된 경우에만 이웃 갱신 분산 하나를 격리 후보로 시험한다. 변경 전후 동일 조건 반복 측정과 정상 낙하·착지·피격·밀침 회귀가 필요하다. 실제 접촉 누락/상태 실패가 나오면 후보를 보류한다.
3. GPU 재생 우회 경로·Mass 대체·flow field·새 군집 지휘는 보류한다. 이 두 저장소를 근거로 현재 진단 전체를 선회하지 않는다.
